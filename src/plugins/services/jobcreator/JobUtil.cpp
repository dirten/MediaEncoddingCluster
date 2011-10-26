#ifndef JOB_FILE
#define JOB_FILE
#include "JobUtil.h"
#include "org/esb/db/hivedb.hpp"
#include "org/esb/config/config.h"
#include "org/esb/av/Encoder.h"
#include "org/esb/hive/CodecFactory.h"
//#include "org/esb/db/Profile.h"
#include "org/esb/io/File.h"
#include "litesql/datasource.hpp"
#include "CodecPropertyTransformer.h"
#include "org/esb/hive/PresetReaderJson.h"
//#include "PresetReaderJson.h"
#include "org/esb/util/StringUtil.h"
#include <map>
#include <vector>
#include "boost/uuid/uuid_generators.hpp"
#include "boost/uuid/uuid_io.hpp"
#include "boost/lexical_cast.hpp"
#include "org/esb/signal/Messenger.h"
#include "org/esb/core/PluginContext.h"

using namespace org::esb::av;
using namespace org::esb::config;

struct JobStreamData {
  int index;
  int in_stream;
  int out_stream;
  AVRational framerate;
  AVRational timebase;
  AVRational codectimebase;
};


namespace org {
  namespace esb {
    namespace plugin {

      JobUtil::JobUtil() {
        org::esb::signal::Messenger::getInstance().addMessageListener(*this);
      }

      JobUtil::~JobUtil() {

      }

      org::esb::core::ServicePlugin::ServiceType JobUtil::getServiceType() {
        return org::esb::core::ServicePlugin::SERVICE_TYPE_SERVER;
      }

      void JobUtil::startService() {
        LOGDEBUG("JobUtil::startService()");

      }

      void JobUtil::stopService() {
        LOGDEBUG("JobUtil::stopService()");

      }

      org::esb::core::OptionsDescription JobUtil::getOptionsDescription() {
        return org::esb::core::OptionsDescription();
      }

      void JobUtil::onMessage(org::esb::signal::Message & msg) {
        if (msg.getProperty<std::string>("jobcreator") == "create") {
          boost::shared_ptr<db::MediaFile> mediafile = msg.getPtrProperty<db::MediaFile >("mediafile");
          boost::shared_ptr<db::Preset> preset =  msg.getPtrProperty<db::Preset >("preset");
          std::string outfile = msg.getProperty<std::string>("outfile");
          int id = createJob(*mediafile.get(), *preset.get(), outfile);
          LOGDEBUG("created job id=" << id);
          msg.setProperty("jobid", id);
          LOGDEBUG("receive to create job:" << outfile);
        }
      }

      /*genau das hier wird benutzt, den rest muss ich löschen*/
      int JobUtil::createJob(db::MediaFile infile, db::Preset preset, std::string outfilename) {
        LOGDEBUG("Create new Job" << preset);
        /*
        if(preset.filename.value().length()==0){
          LOGDEBUG("resolving Preset by Name : "<<preset.name);
          org::esb::io::File presetdir(org::esb::config::Config::get("preset.path"));
          if (presetdir.exists()) {
            FileList files = presetdir.listFiles();
            FileList::iterator file_it = files.begin();
            for (; file_it != files.end(); file_it++) {
              org::esb::hive::PresetReader reader((*file_it)->getPath());
              if(reader.getPreset()["name"]==preset.name.value()){
                preset.filename=(*file_it)->getPath();
                LOGDEBUG("Preset File found : "<<(*file_it)->getPath());
                preset.update();
                break;
              }
            }
          }else{
            LOGWARN("preset directory does not exist:"<<presetdir.getPath());
          }

        }*/
        /**
         * reading the preset from the file
         */
        org::esb::hive::PresetReaderJson reader(preset.data);
        org::esb::hive::PresetReaderJson::CodecList codecs = reader.getCodecList();
        org::esb::hive::PresetReaderJson::FilterList filters = reader.getFilterList();
        org::esb::hive::PresetReaderJson::Preset pre = reader.getPreset();

        LOGDEBUG("searching for Format id:" << pre["id"]);
        /*resolving the outputformat to have knowledge of the global header flag*/
        AVOutputFormat *ofmt = NULL;
        while ((ofmt = av_oformat_next(ofmt))) {
          LOGDEBUG(ofmt->name);
          if (pre["id"] == ofmt->name) {
            break;
          }
        }
        if (!ofmt) {
          LOGERROR("Could not find Output Format");
          return -1;
        }
        if (ofmt->flags & AVFMT_GLOBALHEADER) {
          codecs["video"].insert(std::pair<std::string, std::string > ("global_header", "1"));
          codecs["audio"].insert(std::pair<std::string, std::string > ("global_header", "1"));
        }
        const litesql::Database db = *getContext()->database; //infile.getDatabase();
        db.begin();

        boost::uuids::uuid uuid = boost::uuids::random_generator()();
        std::string uuidstr = boost::lexical_cast<std::string > (uuid);

        db::Job job(db);
        job.created = 0;
        job.begintime = 1;
        job.endtime = 1;
        job.starttime = infile.starttime;
        job.duration = infile.duration;
        job.status = db::Job::Status::Waiting;
        job.infile = infile.path.value() + "/" + infile.filename.value();
        job.data = preset.data.value();
        job.uuid = uuidstr;
        job.update();
        job.preset().link(preset);
        /**
         * creating the output media file
         */
        org::esb::io::File f(infile.filename);
        if (pre.count("fileExtension") > 0)
          f.changeExtension(pre["fileExtension"]);
        else
          f.changeExtension("unknown");

        org::esb::io::File outf(outfilename);

        db::MediaFile outfile(db);
        outfile.filename = outf.getFileName();
        outfile.path = outf.getFilePath();
        outfile.parent = infile.id.value();
        outfile.containertype = ofmt->name;
        outfile.streamcount = 0;
        outfile.update();

        job.outfile = outfile.path.value() + "/" + outfile.filename.value();
        /*
         * setting time data twice, in case of a bug in litesql
         * it does not support zero values,
         * it uses every time the localtime instead of the supplied value
         * but after setting it twice it eat the values ???
         */

        //        job.begintime = 1;
        //        job.endtime = 1;
        //job.outfile = outfile.filename.value();
        job.update();



        vector<db::Stream>streams = infile.streams().get().all();
        vector<db::Stream>::iterator sit = streams.begin();
        CodecPropertyTransformer trans;
        for (int a = 0; sit != streams.end(); sit++) {
          AVMediaType type = static_cast<AVMediaType> ((*sit).streamtype.value());
          if (type != AVMEDIA_TYPE_VIDEO && type != AVMEDIA_TYPE_AUDIO)continue;
          db::Stream s(db);
          s.streamindex = a;
          s.streamtype = (*sit).streamtype.value();
          s.update();
          outfile.streams().link(s);

          /**
           * creating the streamparameter from the profile
           */
          {
            std::string stype = type == AVMEDIA_TYPE_VIDEO ? "video" : "audio";
            std::map<std::string, std::string> codec = codecs[stype];
            std::map<std::string, std::string>::iterator sdata = codec.begin();
            /*
             * handling width and height
             */
            if (type == AVMEDIA_TYPE_VIDEO) {
              /*search for width and height*/
              if (codec.count("width") == 0 ||
                      atoi((*codec.find("width")).second.c_str()) == 0 ||
                      codec.count("height") == 0 ||
                      atoi((*codec.find("height")).second.c_str()) == 0) {
                if (codec.count("width") > 0)
                  codec.erase(codec.find("width"));
                if (codec.count("height") > 0)
                  codec.erase(codec.find("height"));
                codec.insert(std::pair<std::string, std::string > ("width", org::esb::util::StringUtil::toString((*sit).width.value())));
                codec.insert(std::pair<std::string, std::string > ("height", org::esb::util::StringUtil::toString((*sit).height.value())));
              }
              if (codec.count("time_base") == 0 || (*codec.find("time_base")).second.length() == 0) {
                if (codec.count("time_base") > 0)
                  codec.erase(codec.find("time_base"));
                std::ostringstream oss;
                oss << (*sit).framerateden.value() << "/" << (*sit).frameratenum.value();
                codec.insert(std::pair<std::string, std::string > ("time_base", oss.str()));
              }
            }
            for (; sdata != codec.end(); sdata++) {
              db::StreamParameter sp(db);
              if ((*sdata).first == "codec_id") {
                AVCodec * codec = avcodec_find_encoder_by_name((*sdata).second.c_str());
                if (codec)
                  (*sdata).second = org::esb::util::StringUtil::toString(codec->id);
              }
              sp.name = (*sdata).first;
              sp.val = (*sdata).second;
              sp.update();
              s.params().link(sp);
            }
          }
          db::JobDetail jd(db);

          jd.update();
          jd.inputstream().link((*sit));
          jd.outputstream().link(s);
          job.jobdetails().link(jd);

          a++;
        }

        job.inputfile().link(infile);
        job.outputfile().link(outfile);
        db.commit();
        return job.id.value();
      }

    }
  }
}

#endif


