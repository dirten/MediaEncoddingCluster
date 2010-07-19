#ifndef JOB_FILE
#define JOB_FILE
#include "org/esb/db/hivedb.hpp"
#include "org/esb/sql/Connection.h"
#include "org/esb/sql/PreparedStatement.h"
#include "org/esb/sql/Statement.h"
#include "org/esb/sql/ResultSet.h"
#include "org/esb/config/config.h"
#include "org/esb/av/Encoder.h"
#include "org/esb/hive/CodecFactory.h"
#include "org/esb/db/Profile.h"
#include "org/esb/io/File.h"
#include <map>
using namespace org::esb::av;
using namespace org::esb::sql;
using namespace org::esb::config;

struct JobStreamData {
  int index;
  int in_stream;
  int out_stream;
  AVRational framerate;
  AVRational timebase;
  AVRational codectimebase;
};

int jobcreator(int fileid, int profileid, std::string outpath) {
  db::HiveDb db("mysql", org::esb::config::Config::getProperty("db.url"));
//  db.verbose = true;

  db::MediaFile mediafile = litesql::select<db::MediaFile > (db, db::MediaFile::Id == fileid).one();
  db::Profile profile = litesql::select<db::Profile > (db, db::Profile::Id == profileid).one();
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

  db::MediaFile outfile(db);
  outfile.filename = f.getFileName();
  outfile.path = outpath;
  outfile.parent = fileid;
  outfile.containertype = ofmt->name;
  outfile.duration = mediafile.duration;
  outfile.streamcount = mediafile.streamcount;
  outfile.update();

  db::Job job(db);
  job.begintime=0;
  job.endtime=NULL;
  job.update();
  job.inputfile().link(mediafile);
  job.outputfile().link(outfile);

  vector<db::Stream>::iterator it = streams.begin();
  int a = 0;
  for (; it != streams.end(); it++, a++) {
    db::Stream s(db);

    s.streamindex = a;
    s.streamtype = (*it).streamtype;

    if ((*it).streamtype == CODEC_TYPE_VIDEO) {
      s.codecid = (int) profile.vcodec;
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
      s.width = (int) profile.vwidth;
      s.height = (int) profile.vheight;
      s.gopsize = 20;
      boost::shared_ptr<Encoder> enc(new Encoder((CodecID) (int) profile.vcodec));
      enc->setWidth(profile.vwidth);
      enc->setHeight(profile.vheight);
      org::esb::hive::CodecFactory::setCodecOptions(enc, profile.vextra);
      enc->open();
      int flags = enc->getFlags();
      if (ofmt->flags & AVFMT_GLOBALHEADER)
        flags |= CODEC_FLAG_GLOBAL_HEADER;
      s.flags = flags;
      s.pixfmt = (int) enc->getPixelFormat();
      s.bitrate = (int) profile.vbitrate;
      s.extraprofileflags = (std::string)profile.vextra;
    } else if ((*it).streamtype == CODEC_TYPE_AUDIO) {
      s.codecid = (int) profile.acodec;
      s.streamtimebasenum = 1;
      s.streamtimebaseden = (int) profile.asamplerate;
      s.bitrate = (int) profile.abitrate;
      s.samplerate = (int) profile.asamplerate;
      boost::shared_ptr<Encoder> enc(new Encoder((CodecID) (int) profile.acodec));
      enc->setSampleRate(profile.asamplerate);
      enc->setChannels(profile.achannels);
      org::esb::hive::CodecFactory::setCodecOptions(enc, profile.aextra);
      enc->open();
      s.samplefmt = (int) enc->getSampleFormat();
      int flags = enc->getFlags();
      if (ofmt->flags & AVFMT_GLOBALHEADER)
        flags |= CODEC_FLAG_GLOBAL_HEADER;
      s.flags = flags;
      s.bitspercodedsample = (int) enc->getBitsPerCodedSample();
    }
    s.update();
    s.mediafile().link(outfile);
    db::JobDetail job_detail(db);
    job_detail.update();
    job.jobdetails().link(job_detail);
    job_detail.inputstream().link((*it));
    job_detail.outputstream().link(s);

  }
  return job.id;
}

int jobcreator(int argc, char*argv[]) {
  std::map<int, JobStreamData> streams;
  Connection con(Config::getProperty("db.connection"));
  int fileid = atoi(argv[2]), in_v_stream = 0, in_a_stream = 0, v_stream_idx = 0, a_stream_idx = 0;
  long long int outfileid = 0, v_stream_id = 0, a_stream_id = 0, jobid = 0;
  int profileid = atoi(argv[3]);
  int a_stream_num = 0, a_stream_den = 0;

  std::cout << "FileId:" << argv[2] << ":" << std::endl;
  std::cout << "ProfileId:" << argv[3] << ":" << std::endl;


  string filename;

  std::string profileaction = "";
  std::string profilename = "";
  std::string profile_v_format = "";
  int profile_v_codec = 0;
  int profile_v_bitrate = 0;
  std::string profile_v_framerate = "";
  std::string profile_v_extra = "";
  std::string profile_a_extra = "";
  std::string duration = "";
  int profile_v_width = 0;
  int profile_v_height = 0;
  int profile_v_keep_aspect_ratio = 0;
  int profile_v_deinterlace = 0;
  int profile_a_channels = 0;
  int profile_a_codec = 0;
  int profile_a_bitrate = 0;
  int profile_a_samplerate = 0;

  AVOutputFormat *ofmt = NULL;

  //	cout << "FileId:"<<fileid<<"\tProfileId:"<<profileid<<endl;
  {
    PreparedStatement stmt = con.prepareStatement("select id, filename, duration from files where files.id=:id ");
    stmt.setInt("id", fileid);
    ResultSet rs = stmt.executeQuery();
    if (rs.next()) {
      filename = rs.getString("filename");
      duration = rs.getString("duration");
    }
  }
  {
    PreparedStatement stmt = con.prepareStatement("select * from profiles where id=:id ");
    stmt.setInt("id", profileid);
    ResultSet rs = stmt.executeQuery();
    std::cout << "Resolving Profile Data for " << profileid << std::endl;
    if (rs.next()) {
      std::cout << " Profile Data Resolved" << std::endl;

      profilename = rs.getString("profile_name");
      profile_v_format = rs.getString("v_format");
      profile_v_codec = rs.getInt("v_codec");
      profile_v_bitrate = rs.getInt("v_bitrate");
      profile_v_framerate = rs.getString("v_framerate");
      profile_v_width = rs.getInt("v_width");
      profile_v_height = rs.getInt("v_height");
      profile_v_extra = rs.getString("v_extra");
      profile_v_keep_aspect_ratio = rs.getInt("v_keep_aspect_ratio");
      profile_v_deinterlace = rs.getInt("v_deinterlace");
      profile_a_extra = rs.getString("a_extra");
      profile_a_channels = rs.getInt("a_channels");
      profile_a_codec = rs.getInt("a_codec");
      profile_a_bitrate = rs.getInt("a_bitrate");
      profile_a_samplerate = rs.getInt("a_samplerate");
    } else {
      std::cout << " Profile Data not found" << std::endl;
    }
  }

  {
    PreparedStatement stmt = con.prepareStatement("select * from streams where fileid=:id and stream_type=0");
    stmt.setInt("id", fileid);
    ResultSet rs = stmt.executeQuery();
    if (rs.next()) {
      in_v_stream = rs.getInt("id");
      v_stream_idx = rs.getInt("stream_index");
      streams[v_stream_idx].in_stream = rs.getInt("id");
      float f = atof(profile_v_framerate.c_str());
      streams[v_stream_idx].timebase.num = rs.getInt("time_base_num");
      streams[v_stream_idx].timebase.den = rs.getInt("time_base_den");
      streams[v_stream_idx].codectimebase.num = rs.getInt("codec_time_base_num");
      streams[v_stream_idx].codectimebase.den = rs.getInt("codec_time_base_den");
      if (f == 0) {
        AVRational r;
        r.num = rs.getInt("framerate_num");
        r.den = rs.getInt("framerate_den");
        streams[v_stream_idx].framerate = r;
      } else {
        AVRational r = av_d2q(f, 10000);
        /*NOTE:values must be swapped???*/
        streams[v_stream_idx].framerate.den = r.den;
        streams[v_stream_idx].framerate.num = r.num;
      }
    }
  }
  {
    PreparedStatement stmt = con.prepareStatement("select * from streams where fileid=:id and stream_type=1");
    stmt.setInt("id", fileid);
    ResultSet rs = stmt.executeQuery();
    if (rs.next()) {
      in_a_stream = rs.getInt("id");
      a_stream_idx = rs.getInt("stream_index");
      a_stream_num = rs.getInt("time_base_num");
      a_stream_den = rs.getInt("time_base_den");
    }
  }



  {
    PreparedStatement stmt = con.prepareStatement("insert into files ( path, filename, parent, container_type, duration ) values(:path, :filename, :parent,:container, :duration )");
    org::esb::io::File f(filename.c_str());
    std::string ext = f.getExtension();
    while ((ofmt = av_oformat_next(ofmt))) {
      if (profile_v_format == ofmt->long_name) {
        if (ofmt->extensions)
          f.changeExtension(ofmt->extensions);
        else
          f.changeExtension("unknown");
        break;
      }
    }

    //    logerror("Coldnot find the output extension for : "<<profile_v_format);
    stmt.setString("filename", f.getFileName());
    stmt.setString("path", argv[4]);
    stmt.setInt("parent", fileid);
    stmt.setString("container", ofmt->name);
    stmt.setString("duration", duration);
    stmt.execute();
    outfileid = con.lastInsertId();
  }
  if (in_v_stream > 0) {
    PreparedStatement stmt = con.prepareStatement("insert into streams(fileid,stream_index,stream_type,codec,framerate_num,framerate_den, time_base_num, time_base_den, codec_time_base_num, codec_time_base_den, width,height,gop_size,pix_fmt,bit_rate, flags, extra_profile_flags) values"
            "(:fileid, :stream_index, :stream_type, :codec, :framerate_num, :framerate_den, :time_base_num, :time_base_den, :codec_time_base_num, :codec_time_base_den, :width, :height, :gop_size, :pix_fmt, :bit_rate, :flags, :extra_profile_flags)");
    stmt.setLong("fileid", outfileid);
    stmt.setInt("stream_index", 0);
    stmt.setInt("stream_type", 0);
    stmt.setInt("codec", profile_v_codec);
    stmt.setInt("framerate_num", streams[v_stream_idx].framerate.num);
    stmt.setInt("framerate_den", streams[v_stream_idx].framerate.den);
    stmt.setInt("time_base_num", streams[v_stream_idx].timebase.num);
    stmt.setInt("time_base_den", streams[v_stream_idx].timebase.den);
    stmt.setInt("codec_time_base_num", streams[v_stream_idx].codectimebase.num);
    stmt.setInt("codec_time_base_den", streams[v_stream_idx].codectimebase.den);
    stmt.setInt("width", profile_v_width);
    stmt.setInt("height", profile_v_height);
    stmt.setInt("gop_size", 20);
    boost::shared_ptr<Encoder> enc(new Encoder((CodecID) profile_v_codec));
    enc->setWidth(profile_v_width);
    enc->setHeight(profile_v_height);
    org::esb::hive::CodecFactory::setCodecOptions(enc, profile_v_extra);
    enc->open();
    int flags = enc->getFlags();
    if (ofmt->flags & AVFMT_GLOBALHEADER)
      flags |= CODEC_FLAG_GLOBAL_HEADER;

    stmt.setInt("flags", flags);

    stmt.setInt("pix_fmt", enc->getPixelFormat());
    stmt.setInt("bit_rate", profile_v_bitrate);
    stmt.setString("extra_profile_flags", profile_v_extra);
    stmt.execute();
    v_stream_id = con.lastInsertId();
  }
  if (in_a_stream > 0) {
    PreparedStatement stmt = con.prepareStatement("insert into streams(fileid,stream_index,stream_type,codec, time_base_num,time_base_den, bit_rate, sample_rate, channels, sample_fmt, flags, extra_profile_flags) values"
            "(:fileid, :stream_index, :stream_type, :codec, :time_base_num, :time_base_den, :bit_rate, :sample_rate, :channels, :sample_fmt, :flags, :extra_profile_flags)");
    stmt.setLong("fileid", outfileid);
    stmt.setInt("stream_index", 1);
    stmt.setInt("stream_type", 1);
    stmt.setInt("codec", profile_a_codec);
    stmt.setInt("time_base_num", 1);
    stmt.setInt("time_base_den", profile_a_samplerate);
    stmt.setInt("bit_rate", profile_a_bitrate);
    stmt.setInt("sample_rate", profile_a_samplerate);
    stmt.setInt("channels", profile_a_channels);
    stmt.setInt("sample_fmt", 1);
    stmt.setString("extra_profile_flags", profile_a_extra);
    int flags = 0;
    if (ofmt->flags & AVFMT_GLOBALHEADER)
      flags |= CODEC_FLAG_GLOBAL_HEADER;

    stmt.setInt("flags", flags);

    stmt.execute();
    a_stream_id = con.lastInsertId();
  }

  {
    PreparedStatement stmtJob = con.prepareStatement("insert into jobs ( inputfile, outputfile,keep_display_aspect, deinterlace, profileid, status ) values( :inputfile, :outputfile, :keep_display_aspect, :deinterlace, :profileid, 'new')");
    stmtJob.setInt("inputfile", fileid);
    stmtJob.setLong("outputfile", outfileid);
    stmtJob.setLong("profileid", profileid);
    stmtJob.setInt("keep_display_aspect", profile_v_keep_aspect_ratio);
    stmtJob.setInt("deinterlace", profile_v_deinterlace);
    stmtJob.execute();
    jobid = con.lastInsertId();
  }
  {
    PreparedStatement stmtJob = con.prepareStatement("insert into job_details ( job_id,instream, outstream ) values(:jobid, :in, :out)");
    stmtJob.setInt("in", in_v_stream);
    stmtJob.setLong("out", v_stream_id);
    stmtJob.setLong("jobid", jobid);
    stmtJob.execute();
  }
  {
    PreparedStatement stmtJob = con.prepareStatement("insert into job_details ( job_id, instream, outstream ) values( :jobid, :in, :out)");
    stmtJob.setInt("in", in_a_stream);
    stmtJob.setLong("out", a_stream_id);
    stmtJob.setLong("jobid", jobid);
    stmtJob.execute();
  }
  /*there is no need to make process groups into db, because there are not exist anymore*/
  return 0;
  /*
  {
          PreparedStatement stmtJob=con.prepareStatement("insert into frame_groups (jobid, frame_group, startts, stream_id, stream_index, frame_count) (select :jobid, frame_group,min(dts),:id, :stream_idx, count(id) from packets where stream_id=:id group by frame_group)");
          stmtJob.setInt("jobid",jobid);
          stmtJob.setInt("id",in_v_stream);
          stmtJob.execute();
  }
  {
          PreparedStatement stmtJob=con.prepareStatement("insert into frame_groups (jobid, frame_group, startts, stream_id, stream_index, frame_count, complete) (select :jobid, frame_group,min(dts),:id, :stream_idx, count(id), null from packets where stream_id=:id group by frame_group)");
          stmtJob.setInt("jobid",jobid);
          stmtJob.setInt("id",in_a_stream);
          stmtJob.execute();
  }
   */
  {
    PreparedStatement stmtJob = con.prepareStatement("insert into process_units (source_stream, target_stream, start_ts, frame_count) (select stream_id, :target, startts, frame_count from frame_groups where stream_id=:id)");
    //	stmtJob.setInt("jobid",jobid);
    stmtJob.setInt("id", in_v_stream);
    stmtJob.setLong("target", v_stream_id);
    stmtJob.execute();
  }
  {
    PreparedStatement stmtJob = con.prepareStatement("insert into process_units (source_stream, target_stream, start_ts, frame_count) (select stream_id, :target, startts, frame_count from frame_groups where stream_id=:id)");
    //	stmtJob.setInt("jobid",jobid);
    stmtJob.setInt("id", in_a_stream);
    stmtJob.setLong("target", a_stream_id);
    stmtJob.execute();
  }
  return 0;
}
#endif


