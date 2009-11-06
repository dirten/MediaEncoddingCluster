#ifndef JOB_FILE
#  define JOB_FILE
#  include "org/esb/sql/Connection.h"
#  include "org/esb/sql/PreparedStatement.h"
#  include "org/esb/sql/Statement.h"
#  include "org/esb/sql/ResultSet.h"
#  include "org/esb/config/config.h"
#  include "org/esb/av/Codec.h"
using namespace org::esb::av;
using namespace org::esb::sql;
using namespace org::esb::config;

int jobcreator(int argc, char*argv[]) {
  Connection con(Config::getProperty("db.connection"));
  int fileid = atoi(argv[2]), outfileid = 0, v_stream_id = 0, a_stream_id = 0, in_v_stream = 0, in_a_stream = 0, jobid = 0, v_stream_idx = 0, a_stream_idx = 0;
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
  int profile_v_framerate = 0;
  int profile_v_width = 0;
  int profile_v_height = 0;
  int profile_a_channels = 0;
  int profile_a_codec = 0;
  int profile_a_bitrate = 0;
  int profile_a_samplerate = 0;

  //	cout << "FileId:"<<fileid<<"\tProfileId:"<<profileid<<endl;
  {
    PreparedStatement stmt = con.prepareStatement("select id, filename from files where files.id=:id ");
    stmt.setInt("id", fileid);
    ResultSet rs = stmt.executeQuery();
    if (rs.next()) {
      filename = rs.getString("filename");
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
      profile_v_framerate = rs.getInt("v_framerate");
      profile_v_width = rs.getInt("v_width");
      profile_v_height = rs.getInt("v_height");
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
    PreparedStatement stmt = con.prepareStatement("insert into files ( filename ) values( :filename )");
    stmt.setString("filename", profilename + "/" + filename);
    stmt.execute();
    outfileid = con.lastInsertId();
  }
  {
    PreparedStatement stmt = con.prepareStatement("insert into streams(fileid,stream_index,stream_type,codec,framerate_num,framerate_den, time_base_num,time_base_den, width,height,gop_size,pix_fmt,bit_rate) values"
        "(:fileid, :stream_index, :stream_type, :codec, :framerate_num, framerate_den, :time_base_num, :time_base_den, :width, :height, :gop_size, :pix_fmt, :bit_rate)");
    stmt.setInt("fileid", outfileid);
    stmt.setInt("stream_index", 0);
    stmt.setInt("stream_type", 0);
    stmt.setInt("codec", profile_v_codec);
    stmt.setInt("framerate_num", 1);
    stmt.setInt("framerate_den", profile_v_framerate);
    stmt.setInt("time_base_num", 1);
    stmt.setInt("time_base_den", profile_v_framerate);
    stmt.setInt("width", profile_v_width);
    stmt.setInt("height", profile_v_height);
    stmt.setInt("gop_size", 20);
    Codec codec((CodecID) profile_v_codec);
    codec.open();
    stmt.setInt("pix_fmt", codec.getPixelFormat());
    stmt.setInt("bit_rate", profile_v_bitrate);
    stmt.execute();
    v_stream_id = con.lastInsertId();
  }
  {
    PreparedStatement stmt = con.prepareStatement("insert into streams(fileid,stream_index,stream_type,codec, time_base_num,time_base_den, bit_rate, sample_rate, channels, sample_fmt) values"
        "(:fileid, :stream_index, :stream_type, :codec, :time_base_num, :time_base_den, :bit_rate, :sample_rate, :channels, :sample_fmt)");
    stmt.setInt("fileid", outfileid);
    stmt.setInt("stream_index", 1);
    stmt.setInt("stream_type", 1);
    stmt.setInt("codec", profile_a_codec);
    stmt.setInt("time_base_num", a_stream_num);
    stmt.setInt("time_base_den", a_stream_den);
    stmt.setInt("bit_rate", profile_a_bitrate);
    stmt.setInt("sample_rate", profile_a_samplerate);
    stmt.setInt("channels", profile_a_channels);
    stmt.setInt("sample_fmt", 1);
    stmt.execute();
    a_stream_id = con.lastInsertId();
  }

  {
    PreparedStatement stmtJob = con.prepareStatement("insert into jobs ( inputfile, outputfile, begin ) values( :inputfile, :outputfile, now())");
    stmtJob.setInt("inputfile", fileid);
    stmtJob.setInt("outputfile", outfileid);
    stmtJob.execute();
    jobid = con.lastInsertId();
  }
  {
    PreparedStatement stmtJob = con.prepareStatement("insert into job_details ( job_id,instream, outstream ) values(:jobid, :in, :out)");
    stmtJob.setInt("in", in_v_stream);
    stmtJob.setInt("out", v_stream_id);
    stmtJob.setInt("jobid", jobid);
    stmtJob.execute();
  }
  {
    PreparedStatement stmtJob = con.prepareStatement("insert into job_details ( job_id, instream, outstream ) values( :jobid, :in, :out)");
    stmtJob.setInt("in", in_a_stream);
    stmtJob.setInt("out", a_stream_id);
    stmtJob.setInt("jobid", jobid);
    stmtJob.execute();
  }
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
    stmtJob.setInt("target", v_stream_id);
    stmtJob.execute();
  }
  {
    PreparedStatement stmtJob = con.prepareStatement("insert into process_units (source_stream, target_stream, start_ts, frame_count) (select stream_id, :target, startts, frame_count from frame_groups where stream_id=:id)");
    //	stmtJob.setInt("jobid",jobid);
    stmtJob.setInt("id", in_a_stream);
    stmtJob.setInt("target", a_stream_id);
    stmtJob.execute();
  }

}
#endif


