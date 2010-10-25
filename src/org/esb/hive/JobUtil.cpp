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
#include <map>
#include <vector>
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

  vector<db::Stream> streams = mediafile.streams().get().all();

  org::esb::io::File f(mediafile.filename);
  std::string ext = f.getExtension();
  AVOutputFormat *ofmt = NULL;
  while ((ofmt = av_oformat_next(ofmt))) {
    if (profile.format == ofmt->long_name) {
      if (ofmt->extensions)
        f.changeExtension(ofmt->extensions);
      else
        f.changeExtension("unknown");
      break;
    }
  }

  f.changeExtension(profile.formatext);


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
  outfile.containertype = ofmt->name;
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
  job.update();


  job.inputfile().link(mediafile);
  job.outputfile().link(outfile);

  vector<db::Stream>::iterator it = streams.begin();
  int a = 0;
  for (; it != streams.end(); it++) {
    if ((*it).streamtype != CODEC_TYPE_VIDEO && (*it).streamtype != CODEC_TYPE_AUDIO)continue;

    db::Stream s(mediafile.getDatabase());
    s.update();
    s.streamindex = a++;
    s.streamtype = (*it).streamtype;

    if ((*it).streamtype == CODEC_TYPE_VIDEO) {
      //            if (profile.params().get(db::ProfileParameter::Type == AVMEDIA_TYPE_VIDEO).count() > 0) {
      //                vector<db::ProfileParameter> params = profile.params().get(db::ProfileParameter::Type == AVMEDIA_TYPE_VIDEO).all();
      if (profile.params().get().count() > 0) {
        vector<db::ProfileParameter> params = profile.params().get().all();
        vector<db::ProfileParameter>::iterator it = params.begin();
        for (; it != params.end(); it++) {
          db::StreamParameter sp(s.getDatabase());
          sp.name = (*it).name.value();
          sp.val = (*it).val.value();
          sp.update();
          s.params().link(sp);
          if((*it).name.value()=="video_codec_id")
            s.codecid=(*it).val.value();
        }
      }

//      s.codecid = (int) profile.vcodec;
      float f = atof(((std::string)profile.vframerate).c_str());
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
      if (ofmt->flags & AVFMT_GLOBALHEADER)
        flags |= CODEC_FLAG_GLOBAL_HEADER;
      s.flags = flags;
      enc->close();
    } else if ((*it).streamtype == CODEC_TYPE_AUDIO) {
      if (profile.params().get().count() > 0) {
        vector<db::ProfileParameter> params = profile.params().get().all();
        vector<db::ProfileParameter>::iterator it = params.begin();
        for (; it != params.end(); it++) {
          db::StreamParameter sp(s.getDatabase());
          sp.name = (*it).name.value();
          sp.val = (*it).val.value();
          sp.update();
          s.params().link(sp);
          if((*it).name.value()=="audio_codec_id")
            s.codecid=(*it).val.value();
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
      if (ofmt->flags & AVFMT_GLOBALHEADER)
        flags |= CODEC_FLAG_GLOBAL_HEADER;
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
        vector<db::Profile> profiles = p->profiles().get().all();
        vector<db::Filter> filters = p->filter().get().all();

        vector<db::MediaFile>::iterator file_it = files.begin();
        for (; file_it != files.end(); file_it++) {
          db::MediaFile file = (*file_it);
          vector<db::Profile>::iterator profile_it = profiles.begin();
          for (; profile_it != profiles.end(); profile_it++) {
            db::Profile profile = (*profile_it);
            createJob(file, profile, filters, p->outdirectory);
          }
        }
      }

      void JobUtil::createJob(db::MediaFile infile, db::Profile profile, std::vector<db::Filter> filter_vector, std::string outpath) {
        profile.vwidth = 0;
        profile.vheight = 0;
        if (filter_vector.size() > 0) {
          for (int a = 0; a < filter_vector.size(); a++) {
            db::Filter filter = filter_vector[a];
            //            filter.getDatabase().verbose = true;
            if (filter.filterid == "resize") {
              profile.vwidth = atoi(filter.parameter().get(db::FilterParameter::Fkey == "width").one().fval.value().c_str());
              profile.vheight = atoi(filter.parameter().get(db::FilterParameter::Fkey == "height").one().fval.value().c_str());
            }
            if (filter.filterid == "deinterlace") {
              profile.deinterlace = 1;
              //profile.vheight=atoi(filter.parameter().get(db::FilterParameter::Fkey=="height").one());
            }
          }
        }
        jobcreator(infile, profile, outpath);
      }
    }
  }
}

#endif


