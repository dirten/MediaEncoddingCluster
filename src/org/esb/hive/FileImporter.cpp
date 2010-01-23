#ifndef IMPORT_CPP
#  define IMPORT_CPP
//#include <iostream>
//#include <fstream>
#  include <map>
#  include "org/esb/io/File.h"
#  include "org/esb/io/FileOutputStream.h"
#  include "org/esb/av/FormatInputStream.h"
#  include "org/esb/av/PacketInputStream.h"
#  include "org/esb/av/Packet.h"
#  include "org/esb/av/Codec.h"
#  include "org/esb/util/Decimal.h"
#  include "org/esb/hive/DatabaseUtil.h"
//#include <boost/progress.hpp>
//#include <boost/archive/binary_oarchive.hpp>
//#include <boost/archive/binary_iarchive.hpp>
//#include <boost/archive/text_oarchive.hpp>
//#include <boost/archive/text_iarchive.hpp>
//#include <boost/archive/polymorphic_binary_iarchive.hpp> 
//#include <boost/archive/polymorphic_binary_oarchive.hpp> 

#  include "org/esb/io/ObjectOutputStream.h"
#  include "org/esb/config/config.h"
//#include "tntdb/connect.h"
//#include "tntdb/connection.h"
//#include "tntdb/statement.h"
//#include "tntdb/blob.h"
#  include "org/esb/sql/Connection.h"
#  include "org/esb/sql/Statement.h"
#  include "org/esb/sql/PreparedStatement.h"
//#include "org/esb/sql/sqlite3x.hpp"
using namespace std;
using namespace org::esb;
using namespace org::esb::sql;
using namespace org::esb::config;
//using namespace tntdb;
using namespace org::esb::av;
//using namespace boost;
//using namespace sqlite3x;

string toString(int num) {
  char c[10];
  memset(&c, 0, 10);
  sprintf(c, "%d", num);
  return string(c);
}

template < class T > T nullCheck(T param) {
  return param;
}

struct stream_group {
  long long int id; // database stream id
  int idx; // file stream index
  long long int start_ts; // group start ts
  int packet_count; // group packet count
  int stream_type; // audio or video type
  int frame_group; // ???
} ;

int import(int argc, char *argv[]) {
  //	cout << LIBAVCODEC_IDENT << endl;

  if (argc != 2) {
    LOGERROR("org.esb.hive.FileImporter","wrong parameter count");
    exit(1);
  }
  //      Config::init("./cluster.cfg");


//  string connect_str = Config::getProperty("db.connection");
//  logdebug("using db connection : " << connect_str);
  Connection con(Config::getProperty("db.connection"));
  /*
      if (!checkDatabase(con)) {
          createDatabase(con);
      }else {
          cout << "Database found";
      }
   */
  org::esb::io::File inputFile(argv[1]);
  if (!inputFile.exists() || !inputFile.canRead()) {
    LOGERROR("org.esb.hive.FileImporter","Source File not found:"<<inputFile.getPath());
  } else {
    LOGDEBUG("org.esb.hive.FileImporter","File:" << inputFile.getPath());
  }

  long long int fileid = 0, count = 0, frame_group = 0;
  /*	FormatInputStream * fisp=NULL;
    try{
          fisp=new FormatInputStream(&inputFile);
    }catch(Exception & ex){
            logerror(ex.what());
            delete fisp;
            return 0;
    }
   */
  FormatInputStream fis(&inputFile);
  if (!fis.isValid())return 0;

  PacketInputStream pis(&fis);
  fis.getFormatContext()->flags |= AVFMT_FLAG_GENPTS;
  //      tntdb::Connection con=connect(databaseFile.getPath());
  //      sqlite3_transaction trans=con.getTransaction();
  {
    PreparedStatement
    st =
        con.
        prepareStatement("INSERT INTO files(filename, path, size, type, insertdate,stream_count, container_type, title,author, copyright, comment, album, year, track, genre, duration,bitrate) values "
        //          "(?,?,1,now(),?,?,?,?,?,?,?,?,?,?,?)");
        "(:filename,:path, :size,1,now(),:stream_count, :container_type, :title, :author, :copyright, :comment, :album, :year, :track, :genre, :duration, :bitrate)");
    int field = 0;
    string tmp = "testfile.test";
    st.setString("filename", nullCheck < char *>((char *) inputFile.getFileName().c_str()));
    st.setString("path", nullCheck < char *>((char *) inputFile.getFilePath().c_str()));
    st.setDouble("size", (double) fis.getFileSize());
    st.setDouble("stream_count", (double) fis.getStreamCount());
    st.setString("container_type", fis.getFormatContext()->iformat->name);
    st.setString("title", fis.getFormatContext()->title);
    st.setString("author", (char *) fis.getFormatContext()->author);
    st.setString("copyright", (char *) fis.getFormatContext()->copyright);
    st.setString("comment", (char *) fis.getFormatContext()->comment);
    st.setString("album", (char *) fis.getFormatContext()->album);
    st.setInt("year", fis.getFormatContext()->year);
    st.setInt("track", (unsigned int) fis.getFormatContext()->track);
    st.setString("genre", (char *) fis.getFormatContext()->genre);
    st.setDouble("duration", (double) fis.getFormatContext()->duration);
    st.setDouble("bitrate", (double) fis.getFormatContext()->bit_rate);
    st.execute();
    fileid = st.getLastInsertId();
  }
  //      con.executenonquery(string("INSERT INTO files(filename,size) values ( '")+inputFile.getPath()+string("',")+fis.getFileSize()+")");

  AVFormatContext *ctx = fis.getFormatContext();
  const int nb = ctx->nb_streams;
  //  std::map<int, long long int> streams;
  std::map<int, stream_group> stream_groups;
  //  std::map<int, int> codec_types;
  /*
    std::map<int, int> num;
    std::map<int, int> den;
    std::map<int, int> channels;
    std::map<int, int> sample_rates;
    //    std::map<int,int> stream_pts;
    std::map<int, double> stream_start;
   */
  boost::int64_t duration = 0;

  PreparedStatement stmt_fr = con.prepareStatement("insert into frame_groups(frame_group, startts, byte_pos, stream_id, stream_index, frame_count)"
      " values(:frame_group, :startts, :byte_pos, :stream_id, :stream_index, :frame_count)");


  PreparedStatement
  stmt_str =
      con.
      prepareStatement("insert into streams (fileid,stream_index, stream_type,codec, codec_name,framerate_num, framerate_den,start_time,duration,nb_frames,time_base_num, time_base_den, width, height, gop_size, pix_fmt,bit_rate, rate_emu, sample_rate, channels, sample_fmt, bits_per_coded_sample, priv_data_size, priv_data, extra_data_size,extra_data) values "
      //      "(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)");
	  "(:fileid, :stream_index, :stream_type, :codec, :codec_name, :framerate_num, :framerate_den, :start_time, :duration, :nb_frames, :time_base_num, :time_base_den, :width, :height, :gop_size, :pix_fmt, :bit_rate, :rate_emu, :sample_rate, :channels, :sample_fmt,:bits_per_coded_sample, :priv_data_size, :priv_data, :extra_data_size, :extra_data)");
  for (unsigned int a = 0; a < ctx->nb_streams; a++) {
    int field = 0;
    duration += ctx->streams[a]->duration;
    stmt_str.setLong("fileid", fileid);
    stmt_str.setInt("stream_index", (int) a);
    stmt_str.setInt("stream_type", ctx->streams[a]->codec->codec_type);
    stmt_str.setInt("codec", ctx->streams[a]->codec->codec_id);
    //          string tmp=ctx->streams[a]->codec->codec_name;
    stmt_str.setString("codec_name", ctx->streams[a]->codec->codec_name);
    stmt_str.setDouble("framerate_num", ctx->streams[a]->r_frame_rate.num);
    stmt_str.setDouble("framerate_den", ctx->streams[a]->r_frame_rate.den);
    stmt_str.setDouble("start_time", (double) ctx->streams[a]->start_time);
    stmt_str.setDouble("duration", (double) ctx->streams[a]->duration);
    stmt_str.setLong("nb_frames", ctx->streams[a]->nb_frames);
    stmt_str.setInt("time_base_num", ctx->streams[a]->time_base.num);
    stmt_str.setInt("time_base_den", ctx->streams[a]->time_base.den);
    stmt_str.setInt("width", ctx->streams[a]->codec->width);
    stmt_str.setInt("height", ctx->streams[a]->codec->height);
    stmt_str.setInt("gop_size", ctx->streams[a]->codec->gop_size);
    stmt_str.setInt("pix_fmt", ctx->streams[a]->codec->pix_fmt);
    stmt_str.setInt("bit_rate", ctx->streams[a]->codec->bit_rate);
    stmt_str.setInt("rate_emu", ctx->streams[a]->codec->rate_emu);
    stmt_str.setInt("sample_rate", ctx->streams[a]->codec->sample_rate);
    stmt_str.setInt("channels", ctx->streams[a]->codec->channels);
    stmt_str.setInt("sample_fmt", ctx->streams[a]->codec->sample_fmt);
    stmt_str.setInt("bits_per_coded_sample", ctx->streams[a]->codec->bits_per_coded_sample);
	stmt_str.setInt("extra_data_size", ctx->streams[a]->codec->extradata_size);
	stmt_str.setBlob("extra_data", (char*)ctx->streams[a]->codec->extradata,ctx->streams[a]->codec->extradata_size);

    //    stmt_str.setLong("priv_data_size", ctx->streams[a]->codec->codec->priv_data_size);
    //    stmt_str.setBlob("priv_data", (char*)ctx->streams[a]->codec->priv_data, ctx->streams[a]->codec->codec->priv_data_size);




    stmt_str.execute();
    long long int streamid = con.lastInsertId();
    stream_groups[a].id = streamid;
    stream_groups[a].packet_count = 0;
    stream_groups[a].stream_type = ctx->streams[a]->codec->codec_type;
    stream_groups[a].start_ts = -1;
    stream_groups[a].idx = a;
    stream_groups[a].frame_group=0;
    //    streams[a] = streamid;
    //    stream_frame_group[a] = 0;

    /*
        if (ctx->streams[a]->codec->codec_type == CODEC_TYPE_AUDIO) {
          //            stmt_fr.setInt("frame_group",frame_group);
          stmt_fr.setLong("frame_count", ctx->streams[a]->duration);
          stmt_fr.setLong("startts", ctx->streams[a]->start_time);
          stmt_fr.setLong("byte_pos", 0);
          stmt_fr.setLong("stream_id", streamid);
          stmt_fr.setInt("stream_index", a);
          stmt_fr.execute();
        }
     */



    //              Codec codec(ctx->streams[a]->codec->codec_id);

    //          stmt_str.setInt( "priv_data_size", codec._codec->priv_data_size);
    //          avcodec_open(ctx->streams[a]->codec, codec._codec);
    //          stmt_str.setBlob( "priv_data",(char*) ctx->streams[a]->codec->priv_data,codec._codec->priv_data_size);
    //          stmt_str.setInt( "priv_data_size", ctx->iformat->priv_data_size);
    //          stmt_str.setBlob( "priv_data",(char*) ctx->priv_data,ctx->iformat->priv_data_size);
    /*
        codec_types[a] = ctx->streams[a]->codec->codec_type;
        num[a] = ctx->streams[a]->time_base.num;
        den[a] = ctx->streams[a]->time_base.den;
        channels[a] = ctx->streams[a]->codec->channels;
        sample_rates[a] = ctx->streams[a]->codec->sample_rate;
     */
    //        stream_pts[a] = 0;
    //        stream_start[a] = (double) ctx->streams[a]->start_time;

  }
  //      progress_display show_progress(duration);


  /*there is no need to import the whoke file into db*/
  return static_cast<int> (fileid);



  PreparedStatement
  stmt =
      con.prepareStatement("insert into packets(id,stream_id,pts,dts,stream_index,key_frame, frame_group,flags,duration,pos,sort,data_size, data) values "
      //					con.prepareStatement("insert into packets(id,stream_id,pts,dts,stream_index,key_frame, frame_group,flags,duration,pos,data_size) values "
      //    "(NULL,?,?,?,?,?,?,?,?,?,?,?)");
      "(NULL,:stream_id,:pts,:dts,:stream_index,:key_frame, :frame_group,:flags,:duration,:pos,:sort,:data_size, :data)");

  int min_frame_group_count = 5; //atoi(Config::getProperty("hive.min_frame_group_count"));
  int frame_group_counter = 0, next_pts = 0;
  int b_frames_beyond_delay = 3;
  int beyond_delay_counter = 0;
  int BEYOND_STATE = 0;
  long long int startts = 0;
  long long int pkt_count = 0;
  io::File f("test.ldr");
  io::FileOutputStream fos(&f);
  Packet packet;
  cout << endl;
  //      show_progress=duration;
  //  delete fisp;
  //  return static_cast<int> (fileid);



  while (true /*&&count < 100000*/) {
    if (pis.readPacket(packet) < 0) {
      break;
    } else {

    }
    if (++count % 1000 == 0) {
      cout << "\r" << count;
      cout.flush();
    }
    /*
        char * filename=new char[100];

        sprintf(filename,"packet-%d-%d.pkt",packet.getStreamIndex(),count);
      io::File f(filename);
      io::FileOutputStream fos(&f);
      io::ObjectOutputStream oos(&fos);
      oos.writeObject(packet);
      fos.close();
     */
    if (stream_groups[packet.packet->stream_index].start_ts == -1)
      stream_groups[packet.packet->stream_index].start_ts = packet.getDts();

    stream_groups[packet.packet->stream_index].packet_count++;
    //    if (packet.packet->stream_index == 0)
    //      frame_group_counter++;


    if (packet.isKeyFrame() &&
        stream_groups[packet.packet->stream_index].packet_count >= min_frame_group_count *
        (stream_groups[packet.packet->stream_index].stream_type == CODEC_TYPE_AUDIO ? 100 : 1)) {
      stmt_fr.setLong("frame_group", stream_groups[packet.packet->stream_index].frame_group);
      stmt_fr.setInt("frame_count", stream_groups[packet.packet->stream_index].packet_count);
      stmt_fr.setLong("startts", stream_groups[packet.packet->stream_index].start_ts);
      stmt_fr.setLong("byte_pos", 0);
      stmt_fr.setLong("stream_id", stream_groups[packet.packet->stream_index].id);
      stmt_fr.setInt("stream_index", stream_groups[packet.packet->stream_index].idx);
      stmt_fr.execute();
      stream_groups[packet.packet->stream_index].frame_group++;
      stream_groups[packet.packet->stream_index].start_ts=packet.getDts();
      stream_groups[packet.packet->stream_index].packet_count = 0;
    }

    int field = 0;
    packet.packet->duration = packet.packet->duration == 0 ? 1
        : packet.packet->duration;
    stmt.setLong("stream_id", stream_groups[packet.packet->stream_index].id);
    stmt.setLong("pts", packet.packet->pts);
    //		stmt.setDouble("pts", (double) stream_pts[packet.packet->stream_index]);
    stmt.setLong("dts", packet.packet->dts);
    stmt.setInt("stream_index", packet.packet->stream_index);
    stmt.setInt("key_frame", packet.isKeyFrame());
    if (packet.packet->stream_index == 0)
      stmt.setLong("frame_group", frame_group);
    else
      stmt.setInt("frame_group", 0);
    stmt.setInt("flags", packet.packet->flags);
    stmt.setInt("duration", packet.packet->duration);
    stmt.setLong("pos", packet.packet->pos);
    stmt.setLong("sort", ++pkt_count);
    stmt.setInt("data_size", packet.packet->size);

    //    Blob blob((const char*) packet.data, packet.size);
    stmt.setBlob("data", (char*) packet.packet->data, packet.packet->size);
    stmt.execute();

    //      show_progress+=packet.duration;

    /*
            if (codec_types[packet.packet->stream_index] == CODEC_TYPE_VIDEO) {
                            stream_pts[packet.packet->stream_index] += ((boost::int64_t) 1000000 * num[packet.packet->stream_index]) / den[packet.packet->stream_index];
            }

            if (codec_types[packet.packet->stream_index] == CODEC_TYPE_AUDIO) {
                            stream_pts[packet.packet->stream_index] += ((boost::int64_t) AV_TIME_BASE / 2 * packet.packet->size) /
                        (sample_rates[packet.packet->stream_index] * channels[packet.packet->stream_index]);
            }
     */

  }
  
    for(unsigned int a=0;a<stream_groups.size();a++){
      stmt_fr.setLong("frame_group", stream_groups[a].frame_group);
      stmt_fr.setInt("frame_count", stream_groups[a].packet_count);
      stmt_fr.setLong("startts", stream_groups[a].start_ts);
      stmt_fr.setLong("byte_pos", 0);
      stmt_fr.setLong("stream_id", stream_groups[a].id);
      stmt_fr.setInt("stream_index", stream_groups[a].idx);
      stmt_fr.execute();
    }
   

  //    trans.commit();
  cout << endl;
  //      show_progress=duration;
  //  delete fisp;
  return static_cast<int> (fileid);
}

/*
 int main(int argc, char * argv[]){
 import(argc,argv);
 }
 */
#endif
