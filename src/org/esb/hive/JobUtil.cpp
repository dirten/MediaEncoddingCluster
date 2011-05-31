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
#include "PresetReader.h"
#include "PresetReaderJson.h"
#include "org/esb/util/StringUtil.h"
#include <map>
#include <vector>
#include "boost/uuid/uuid_generators.hpp"
#include "boost/uuid/uuid_io.hpp"
#include "boost/lexical_cast.hpp"
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

/*
int jobcreator(int fileid, int profileid, std::string outpath) {
  db::HiveDb db("mysql", org::esb::config::Config::getProperty("db.url"));
  db::MediaFile mediafile = litesql::select<db::MediaFile > (db, db::MediaFile::Id == fileid).one();
  db::Profile profile = litesql::select<db::Profile > (db, db::Profile::Id == profileid).one();
  return jobcreator(mediafile, profile, outpath);
}*/

int jobcreator(db::MediaFile mediafile, db::Profile profile, std::string outpath) {
  mediafile.getDatabase().begin();
  vector<db::Stream> streams = mediafile.streams().get().all();

  vector<db::ProfileParameter>vparams = profile.params().get().all();
  std::map<std::string, Ptr<db::ProfileParameter> > parameter;
  vector<db::ProfileParameter>::iterator vp = vparams.begin();
  for (; vp != vparams.end(); vp++) {
    parameter[(*vp).name] = new db::ProfileParameter(*vp);
  }
  AVOutputFormat *ofmt = NULL;
  while ((ofmt = av_oformat_next(ofmt))) {
    if (parameter["file_format"]->val == ofmt->name) {
      break;
    }
  }
  org::esb::io::File f(mediafile.filename);
  f.changeExtension(parameter["fileExtension"]->val);


  db::Job job(mediafile.getDatabase());
  job.created = 0;
  job.begintime = 1;
  job.endtime = 1;
  job.starttime = mediafile.starttime;
  job.duration = mediafile.duration;
  job.status = "queued";
  job.infile = mediafile.filename.value();
  job.update();

  db::JobLog log(job.getDatabase());
  log.message = "Job Enqueued, is waiting for the next free Encoding slot";
  log.update();
  job.joblog().link(log);

  db::MediaFile outfile(mediafile.getDatabase());
  std::string filename = job.id;
  filename += "#";
  filename += f.getFileName();
  outfile.filename = filename;
  outfile.path = outpath + "/" + profile.name;
  outfile.parent = mediafile.id.value();
  outfile.containertype = parameter["file_format"]->val;
  //  outfile.duration = mediafile.duration;
  outfile.streamcount = mediafile.streamcount;
  outfile.update();
  //  outfile.project().link(mediafile.project().get().one());
  if (mediafile.project().get().count() > 0 && mediafile.project().get().one().filter().get().count() > 0) {
    vector<db::Filter> filters = mediafile.project().get().one().filter().get().all();
    vector<db::Filter>::iterator filter_it = filters.begin();
    for (; filter_it != filters.end(); filter_it++) {
      outfile.filter().link((*filter_it));
    }
  }
  job.outfile = outfile.filename.value();
  /*
   * setting time data twice, in case of a bug in litesql
   * it does not support zero values,
   * it uses every time the localtime instead of the supplied value
   * but after setting it twice it eat the values ???
   */

  job.begintime = 1;
  job.endtime = 1;
  job.outfile = outfile.filename.value();
  job.update();


  job.inputfile().link(mediafile);
  job.outputfile().link(outfile);

  vector<db::Stream>::iterator it = streams.begin();
  int a = 0;
  for (; it != streams.end(); it++) {
    if ((*it).streamtype != AVMEDIA_TYPE_VIDEO && (*it).streamtype != AVMEDIA_TYPE_AUDIO)continue;

    db::Stream s(mediafile.getDatabase());
    s.update();
    s.streamindex = a++;
    s.streamtype = (*it).streamtype;

    if ((*it).streamtype == AVMEDIA_TYPE_VIDEO) {
      if (profile.params().get().count() > 0) {
        vector<db::ProfileParameter> params = profile.params().get().all();
        vector<db::ProfileParameter>::iterator it = params.begin();
        for (; it != params.end(); it++) {
          db::StreamParameter sp(s.getDatabase());
          sp.name = (*it).name.value();
          sp.val = (*it).val.value();
          sp.update();
          s.params().link(sp);
          if ((*it).name.value() == "video_codec_id")
            s.codecid = (*it).val.value();
        }
      }

      float f = atof(((std::string)parameter["frame_rate"]->val).c_str());
      if (f == 0) {
        s.frameratenum = (int) (*it).frameratenum; //rs.getInt("framerate_num");
        s.framerateden = (int) (*it).framerateden; //rs.getInt("framerate_den");
      } else {
        AVRational r = av_d2q(f, 10000);
        /*NOTE:values must be swapped???*/
        s.framerateden = r.den;
        s.frameratenum = r.num;
      }
      s.streamtimebasenum = (int) (*it).streamtimebasenum;
      s.streamtimebaseden = (int) (*it).streamtimebaseden;
      s.codectimebasenum = (int) (*it).codectimebasenum;
      s.codectimebaseden = (int) (*it).codectimebaseden;

      if (profile.vwidth.value() > 0) {
        s.width = (int) profile.vwidth;
      } else {
        s.width = (*it).width.value();
      }

      if (profile.vheight.value() > 0) {
        s.height = (int) profile.vheight;
      } else {
        s.height = (*it).height.value();
      }

      s.gopsize = 20;
      s.bitrate = (int) profile.vbitrate;
      s.extraprofileflags = (std::string)profile.vextra;

      boost::shared_ptr<Encoder> enc(new Encoder((CodecID) (int) s.codecid.value()));
      enc->setWidth(s.width);
      enc->setHeight(s.height);
      org::esb::hive::CodecFactory::setCodecOptions(enc, s.params().get().all());
      enc->open();
      s.pixfmt = (int) enc->getPixelFormat();
      int flags = enc->getFlags();
      if (ofmt->flags & AVFMT_GLOBALHEADER) {
        db::StreamParameter sp(s.getDatabase());
        sp.name = "global_header";
        sp.val = "1";
        sp.update();
        s.params().link(sp);
      }
      s.flags = flags;
      enc->close();
    } else if ((*it).streamtype == AVMEDIA_TYPE_AUDIO) {
      if (profile.params().get().count() > 0) {
        vector<db::ProfileParameter> params = profile.params().get().all();
        vector<db::ProfileParameter>::iterator it = params.begin();
        for (; it != params.end(); it++) {
          db::StreamParameter sp(s.getDatabase());
          sp.name = (*it).name.value();
          sp.val = (*it).val.value();
          sp.update();
          s.params().link(sp);
          if ((*it).name.value() == "audio_codec_id")
            s.codecid = (*it).val.value();
        }
      }
      //      s.codecid = (int) profile.acodec;
      s.streamtimebasenum = 1;
      s.streamtimebaseden = (int) profile.asamplerate;
      s.bitrate = (int) profile.abitrate;
      s.samplerate = (int) profile.asamplerate;
      boost::shared_ptr<Encoder> enc(new Encoder((CodecID) (int) profile.acodec));
      enc->setSampleRate(profile.asamplerate);
      enc->setChannels(profile.achannels);
      org::esb::hive::CodecFactory::setCodecOptions(enc, s.params().get().all());
      enc->open();
      s.channels = profile.achannels.value();
      s.samplefmt = 1; //(int) enc->getSampleFormat();
      int flags = enc->getFlags();
      if (ofmt->flags & AVFMT_GLOBALHEADER) {
        db::StreamParameter sp(s.getDatabase());
        sp.name = "global_header";
        sp.val = "1";
        sp.update();
        s.params().link(sp);
      }
      s.flags = flags;
      s.bitspercodedsample = (int) enc->getBitsPerCodedSample();
      enc->close();
    }
    s.update();
    s.mediafile().link(outfile);
    db::JobDetail job_detail(mediafile.getDatabase());
    job_detail.deinterlace = profile.deinterlace.value();
    job_detail.update();
    job.jobdetails().link(job_detail);
    job_detail.inputstream().link((*it));
    job_detail.outputstream().link(s);

  }
  mediafile.getDatabase().commit();
  return job.id;
}


namespace org {
  namespace esb {
    namespace hive {

      JobUtil::JobUtil() {

      }

      JobUtil::~JobUtil() {

      }

      void JobUtil::createJob(Ptr<db::Project> p) {
        vector<db::MediaFile> files = p->mediafiles().get().all();
        vector<db::Preset> presets = p->presets().get().all();
        vector<db::Filter> filters = p->filter().get().all();

        vector<db::MediaFile>::iterator file_it = files.begin();
        for (; file_it != files.end(); file_it++) {
          db::MediaFile file = (*file_it);
          vector<db::Preset>::iterator preset_it = presets.begin();
          for (; preset_it != presets.end(); preset_it++) {
            db::Preset preset = (*preset_it);
            createJob(file, preset, p->outdirectory);
          }
        }
      }

      int JobUtil::createJob(db::MediaFile infile, db::Preset preset, std::string outpath) {
        LOGDEBUG("Create new Job");
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
        PresetReaderJson reader(preset.data);
        PresetReaderJson::CodecList codecs = reader.getCodecList();
        PresetReaderJson::FilterList filters = reader.getFilterList();
        PresetReaderJson::Preset pre = reader.getPreset();


        /*resolving the outputformat to have knowledge of the global header flag*/
        AVOutputFormat *ofmt = NULL;
        while ((ofmt = av_oformat_next(ofmt))) {
          if (pre["id"] == ofmt->name) {
            break;
          }
        }
        if(!ofmt){
          LOGERROR("Could not find Output Format");
          return -1;
        }

        const litesql::Database db = infile.getDatabase();
        db.begin();

        boost::uuids::uuid uuid = boost::uuids::random_generator()();
        std::string uuidstr = boost::lexical_cast<std::string > (uuid);

        db::Job job(db);
        job.created = 0;
        job.begintime = 1;
        job.endtime = 1;
        job.starttime = infile.starttime;
        job.duration = infile.duration;
        job.status = "queued";
        job.infile = infile.path.value()+"/"+infile.filename.value();
        job.data=preset.data.value();
        job.uuid=uuidstr;
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

        db::MediaFile outfile(db);
        std::string filename = job.id;
        filename += "#";
        filename += f.getFileName();
        outfile.filename = filename;
        outfile.path = outpath + pre["name"];
        outfile.parent = infile.id.value();
        outfile.containertype = ofmt->name;
        outfile.streamcount = 0;
        outfile.update();

        job.outfile = outfile.path.value()+"/"+outfile.filename.value();
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
        org::esb::hive::CodecPropertyTransformer trans;
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
            std::multimap<std::string, std::string> codec = codecs[stype];
            std::multimap<std::string, std::string>::iterator sdata = codec.begin();
            /*
             * handling width and height
             */
            if(type == AVMEDIA_TYPE_VIDEO){
              /*search for width and height*/
              if(codec.count("width")==0||codec.count("height")==0){
                codec.insert(std::pair<std::string, std::string>("width",org::esb::util::StringUtil::toString((*sit).width.value())));
                codec.insert(std::pair<std::string, std::string>("height",org::esb::util::StringUtil::toString((*sit).height.value())));
              }              
            }
            for (; sdata != codec.end(); sdata++) {
              db::StreamParameter sp(db);
              if((*sdata).first=="codec_id"){
                 AVCodec * codec=avcodec_find_encoder_by_name((*sdata).second.c_str());
                 if(codec)
                   (*sdata).second=org::esb::util::StringUtil::toString(codec->id);
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
        /*handling filter for the output media file*/
        PresetReader::FilterList::iterator filter_it=filters.begin();
        for(;filter_it!=filters.end();filter_it++){
          std::string filterid=(*filter_it).first;
          db::Filter f(db);
          f.filterid=filterid;
          f.update();
          outfile.filter().link(f);
          /*if the filter has paramater*/
          if(filters[filterid].size()>0){
            std::map<std::string, std::string> parameter(filters[filterid].begin(),filters[filterid].end());
            std::map<std::string, std::string>::iterator pa_it=parameter.begin();
            for(;pa_it!=parameter.end();pa_it++){
              db::FilterParameter pa(db);
              pa.fkey=(*pa_it).first;
              pa.fval=(*pa_it).second;
              pa.update();
              f.parameter().link(pa);
            }
          }
        }

        job.inputfile().link(infile);
        job.outputfile().link(outfile);
        db.commit();
        return job.id.value();
      }

      int JobUtil::createJob(db::MediaFile infile, db::Profile profile, std::string outpath) {

        const litesql::Database db = infile.getDatabase();
        db.begin();


        db::Job job(db);
        job.created = 0;
        job.begintime = 1;
        job.endtime = 1;
        job.starttime = infile.starttime;
        job.duration = infile.duration;
        job.status = "queued";
        job.infile = infile.filename.value();
        
        job.update();


        /**
         * copy the profile for the new Job
         */
        db::Profile newProfile(db);
        newProfile.name = profile.name.value();
        newProfile.update();

        /**
         * copy the profile parameter for the new Job
         */
        {
          vector<db::ProfileParameter>params = profile.params().get().all();
          vector<db::ProfileParameter>::iterator it = params.begin();
          for (; it != params.end(); it++) {
            db::ProfileParameter p(db);
            p.name = (*it).name.value();
            p.val = (*it).val.value();
            p.mediatype = (*it).mediatype.value();
            p.update();
            newProfile.params().link(p);
          }
        }
        /**
         * copy the Filter from the profile for the new Job
         */
        {
          vector<db::Filter> filters = profile.filter().get().all();
          vector<db::Filter>::iterator it = filters.begin();
          for (; it != filters.end(); it++) {
            db::Filter f(db);
            f.filterid = (*it).filterid.value();
            f.filtername = (*it).filtername.value();
            f.update();
            newProfile.filter().link(f);
            /**
             * copy the FilterParameter from the Filter for the new Job
             */
            vector<db::FilterParameter>params = (*it).parameter().get().all();
            vector<db::FilterParameter>::iterator it_p = params.begin();
            for (; it_p != params.end(); it_p++) {
              db::FilterParameter fp(db);
              fp.fkey = (*it_p).fkey.value();
              fp.fval = (*it_p).fval.value();
              fp.update();
              f.parameter().link(fp);
            }
          }
        }
        //job.profile().link(newProfile);

        /**
         * preparing the profile data into a map
         */
        vector<db::ProfileParameter>vparams = profile.params().get().all();
        std::map<std::string, Ptr<db::ProfileParameter> > parameter;
        vector<db::ProfileParameter>::iterator vp = vparams.begin();
        for (; vp != vparams.end(); vp++) {
          parameter[(*vp).name] = new db::ProfileParameter(*vp);
        }

        AVOutputFormat *ofmt = NULL;
        while ((ofmt = av_oformat_next(ofmt))) {
          if (parameter["file_format"]->val.value() == ofmt->name) {
            break;
          }
        }

        /**
         * creating the output media file
         */
        org::esb::io::File f(infile.filename);
        if (parameter.count("fileExtension") > 0)
          f.changeExtension(parameter["fileExtension"]->val);
        else
          f.changeExtension("unknown");


        db::MediaFile outfile(db);
        std::string filename = job.id;
        filename += "#";
        filename += f.getFileName();
        outfile.filename = filename;
        outfile.path = outpath + "/" + profile.name;
        outfile.parent = infile.id.value();
        outfile.containertype = parameter["file_format"]->val.value();
        outfile.streamcount = 0;
        outfile.update();


        vector<db::Stream>streams = infile.streams().get().all();
        vector<db::Stream>::iterator sit = streams.begin();
        org::esb::hive::CodecPropertyTransformer trans;
        for (int a = 0; sit != streams.end(); sit++, a++) {
          db::Stream s(db);
          s.streamindex = a;
          s.streamtype = (*sit).streamtype.value();
          s.update();
          outfile.streams().link(s);
          /**
           * creating the streamparameter from the profile
           */
          {
            std::map<std::string, AVMediaType> type_map;
            type_map["width"] = AVMEDIA_TYPE_VIDEO;
            type_map["height"] = AVMEDIA_TYPE_VIDEO;
            vector<db::ProfileParameter>params = profile.params().get().all();
            vector<db::ProfileParameter>::iterator it = params.begin();
            for (; it != params.end(); it++) {
              bool create = false;
              const AVOption * option = trans.getOption((*it).name.value());
              if (option && option->flags & ((*sit).streamtype.value() == AVMEDIA_TYPE_VIDEO ? AV_OPT_FLAG_VIDEO_PARAM : AV_OPT_FLAG_AUDIO_PARAM)) {
                create = true;
              } else
                if (option && option->flags == 0) {
                create = true;
              } else
                if ((*it).name.value() == "audio_codec_id" && (*sit).streamtype.value() == AVMEDIA_TYPE_AUDIO) {
                (*it).name = "codec_id";
                create = true;
              } else
                if ((*it).name.value() == "video_codec_id" && (*sit).streamtype.value() == AVMEDIA_TYPE_VIDEO) {
                (*it).name = "codec_id";
                create = true;
              } else
                if (type_map.count((*it).name.value()) > 0 && (*sit).streamtype.value() == type_map[(*it).name.value()]) {
                create = true;
              }
              if (create) {
                db::StreamParameter sp(db);
                sp.name = (*it).name.value();
                sp.val = (*it).val.value();
                sp.update();
                s.params().link(sp);
              }
            }
          }
          if (ofmt->flags & AVFMT_GLOBALHEADER) {
            db::StreamParameter sp(s.getDatabase());
            sp.name = "global_header";
            sp.val = "1";
            sp.update();
            s.params().link(sp);
          }
          try {
            if (false && profile.filter().get(db::Filter::Filterid == "resize").count() > 0) {
              db::StreamParameter spw(s.getDatabase());
              spw.name = "width";
              spw.val = profile.filter().get(db::Filter::Filterid == "resize").one().parameter().get(db::FilterParameter::Fkey == "width").one().fval.value();
              spw.update();
              s.params().link(spw);
              db::StreamParameter sph(s.getDatabase());
              sph.name = "height";
              sph.val = profile.filter().get(db::Filter::Filterid == "resize").one().parameter().get(db::FilterParameter::Fkey == "height").one().fval.value();
              sph.update();
              s.params().link(sph);
            }
          } catch (litesql::NotFound ex) {
            LOGERROR(ex.what());
          }
          db::JobDetail jd(db);

          jd.update();
          jd.inputstream().link((*sit));
          jd.outputstream().link(s);
          job.jobdetails().link(jd);
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


