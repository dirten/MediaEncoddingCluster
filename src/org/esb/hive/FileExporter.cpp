#include "FileExporter.h"
#include "org/esb/io/File.h"
#include "org/esb/io/FileInputStream.h"
#include "org/esb/io/ObjectInputStream.h"
#include "org/esb/hive/job/ProcessUnit.h"

#include "org/esb/sql/Connection.h"
#include "org/esb/sql/Statement.h"
#include "org/esb/sql/ResultSet.h"
#include "org/esb/av/PacketOutputStream.h"
#include "org/esb/av/Packet.h"
#include "org/esb/av/Decoder.h"
#include "org/esb/av/FormatOutputStream.h"
#include "org/esb/io/File.h"
#include "org/esb/config/config.h"
#include "org/esb/hive/CodecFactory.h"
#include <boost/filesystem/exception.hpp>
#include <map>
using namespace org::esb::av;
using namespace org::esb::io;
using namespace org::esb::lang;
using namespace org::esb::sql;
using namespace org::esb::hive;
using namespace org::esb::config;

std::map<int, FileExporter::StreamData> FileExporter::_source_stream_map;

void FileExporter::exportFile(int fileid) {


  map<int, boost::shared_ptr<Encoder> > enc;
  map<int, int> ptsmap;
  map<int, int> dtsmap;
  map<int, long long int> ptsoffset;
  map<int, long long int> dtsoffset;
  //  map<int, int> dtsmap;
  logdebug("Exporting file with id:" << fileid);
  std::string filename;
  std::string fileformat;
  Connection con(std::string(Config::getProperty("db.connection")));

  {
    PreparedStatement stmt = con.prepareStatement("select * from files where files.id=:id");
    stmt.setInt("id", fileid);
    ResultSet rs = stmt.executeQuery();
    if (rs.next()) {
      fileformat = rs.getString("container_type");
      if (rs.getString("path").size() == 0) {
        filename = Config::getProperty("hive.base_path");
      } else {
        filename = rs.getString("path");
      }
      filename += "/";
      filename += rs.getString("filename");
    }
  }
  org::esb::io::File fout(filename.c_str());
  org::esb::io::File outDirectory(fout.getFilePath().c_str());
  if (!outDirectory.exists()) {
    logdebug("creating output directory:" << outDirectory.getFilePath());
    try {
      outDirectory.mkdir();
    } catch (boost::filesystem::filesystem_error & e) {
      logerror(e.what());
      return;
    }
  }
  //  string stream_id = fileid;
  FormatOutputStream * fos = new FormatOutputStream(&fout, fileformat.c_str());
  PacketOutputStream * pos = new PacketOutputStream(fos);

  int video_id = 0;
  int audio_id = 0;
  int audio_pts = 0;
  int video_pts = 0;
  bool build_offset = true;
  map<int, int> stream_map;
  int64_t min_start_time = 0;
  AVRational basear;
  basear.num = 1;
  basear.den = 1000000;

  //    int v_num=0,v_den=0,a_num=0,a_den=0;
  {
    PreparedStatement stmt = con.prepareStatement("select sin.stream_index inid, sout.stream_index outid,sin.start_time in_start_time, sin.time_base_num intbnum, sin.time_base_den intbden from jobs, job_details, streams sin, streams sout where jobs.id=job_details.job_id and sin.id=job_details.instream and sout.id=job_details.outstream and outputfile=:fileid");
    stmt.setInt("fileid", fileid);
    ResultSet rs = stmt.executeQuery();
    while (rs.next()) {
      _source_stream_map[rs.getInt("inid")].out_stream_index = rs.getInt("outid");
      _source_stream_map[rs.getInt("inid")].in_start_time = rs.getLong("in_start_time");
      AVRational ar;
      ar.num = rs.getInt("intbnum");
      ar.den = rs.getInt("intbden");
      _source_stream_map[rs.getInt("inid")].in_timebase = ar;

      if (min_start_time < av_rescale_q(rs.getLong("in_start_time"), ar, basear)) {
        min_start_time = av_rescale_q(rs.getLong("in_start_time"), ar, basear);
      }
    }
  }
  {
    PreparedStatement stmt = con.prepareStatement("select *, streams.id as sid from files, streams where files.id=:id and streams.fileid=files.id and streams.id limit 2");
    stmt.setInt("id", fileid);
    ResultSet rs = stmt.executeQuery();

    while (rs.next()) {
      boost::shared_ptr<Encoder> codec = CodecFactory::getStreamEncoder(rs.getInt("sid"));
      codec->open();
      //      Encoder *encoder=CodecFactory::getStreamEncoder(rs.getInt("sid"));
      //      encoder->open();

      enc[rs.getInt("stream_index")] = codec;
      ptsmap[rs.getInt("stream_index")] = 0;
      dtsmap[rs.getInt("stream_index")] = 0;
      dtsoffset[rs.getInt("stream_index")] = -1;
      ptsoffset[rs.getInt("stream_index")] = -1;
      pos->setEncoder(*codec, rs.getInt("stream_index"));
      logdebug("Added Encoder to StreamIndex:" << rs.getInt("stream_index"));
      if (rs.getInt("stream_type") == CODEC_TYPE_VIDEO) {
        video_id = rs.getInt("sid");
      }

      if (rs.getInt("stream_type") == CODEC_TYPE_AUDIO) {
        audio_id = rs.getInt("sid");
      }
      if (codec->getCodecId() == CODEC_ID_MPEG2VIDEO) {
        build_offset = false;
      }

      /*
      CodecID codec_id = (CodecID) rs.getInt("codec");
      if (rs.getInt("stream_type") == CODEC_TYPE_AUDIO) {
  //      codec_id = av_guess_codec(fos._fmtCtx->oformat, NULL, fout.getPath().c_str(), NULL, CODEC_TYPE_AUDIO);

      }

      encoder = new Encoder(codec_id);
      encoder->setBitRate(rs.getInt("bit_rate"));
      AVRational ar;
      ar.den = rs.getInt("time_base_den");
      ar.num = rs.getInt("time_base_num");
      encoder->setTimeBase(ar);
      encoder->setGopSize(rs.getInt("gop_size"));
      encoder->setPixelFormat((PixelFormat) rs.getInt("pix_fmt"));
      encoder->setWidth(rs.getInt("width"));
      encoder->setHeight(rs.getInt("height"));
      encoder->setSampleRate(rs.getInt("sample_rate"));
      encoder->setChannels(rs.getInt("channels"));
      encoder->setSampleFormat((SampleFormat) rs.getInt("sample_fmt"));
      encoder->open();
      if (rs.getInt("stream_type") == CODEC_TYPE_VIDEO) {
        video_id = rs.getInt("sid");
        //        cout <<"VideoId"<<video_id<<"\taudio_id"<<audio_id<<endl;
        //        encoder->ctx->block_align=1;
        //        v_num=rs.getInt("time_base_num");
        //        v_den=rs.getInt("time_base_den");
        pos.setEncoder(*encoder, rs.getInt("stream_index"));
      }
      if (rs.getInt("stream_type") == CODEC_TYPE_AUDIO) {
        audio_id = rs.getInt("sid");
        //        cout <<"VideoId"<<video_id<<"\taudio_id"<<audio_id<<endl;
        //        encoder->setTimeBase((AVRational){3,125});
        //            encoder->open();
        //        cout << "Num:"<<encoder->ctx->time_base.num<<"\tDen:"<<encoder->ctx->time_base.den<<endl;
        //        a_num=rs.getInt("time_base_num");
        //        a_den=rs.getInt("time_base_den");
        //        encoder->ctx->block_align=1;
        pos.setEncoder(*encoder, rs.getInt("stream_index"));
      }
       */
      cout << "StreamTimeBaseNum:" << fos->_fmtCtx->streams[rs.getInt("stream_index")]->time_base.num;
      cout << "\tStreamTimeBaseDen:" << fos->_fmtCtx->streams[rs.getInt("stream_index")]->time_base.den;
      cout << "CodecTimeBaseNum:" << fos->_fmtCtx->streams[rs.getInt("stream_index")]->codec->time_base.num;
      cout << "\tCodecTimeBaseDen:" << fos->_fmtCtx->streams[rs.getInt("stream_index")]->codec->time_base.den;
      //    logdebug("neue Zeile");
      cout.flush();
    }
  }
  //	fos.open();
  pos->init();

  /**
   * check if encoding is full finished
   */
  {
    //    string sql="select a.data_size, a.data, a.pts, a.dts, a.duration, a.flags, a.pos, a.stream_index from packets a where a.stream_id=";
    //    sql+=stream_id;
    //    sql+=" order by a.pts limit 5000";
    //select * from packets where stream_id in(1,2) order by case when stream_id=1 then 1000/25000*pts else 1/16000*pts end;
    //select * from packets, streams s where stream_id=s.id and stream_id in (3,4)  order by s.time_base_num/s.time_base_den*pts
    //    string sql = "select data, data_size, packets.stream_index,packets.flags,packets.duration, dts, pts from packets, streams s where stream_id=s.id and stream_id in (:video,:audio) order by s.time_base_num*dts/s.time_base_den";
    std::string sql = "select * from process_units where target_stream in (:video, :audio) order by id";

    //    string sql = "select * from packets, streams s where stream_id=s.id and stream_id in (:video,:audio) order by sort";
    //        string sql="select * from packets, streams s where stream_id=s.id and stream_id in (:video, :audio) order by dts";
    PreparedStatement stmt = con.prepareStatement(sql.c_str());
    stmt.setInt("video", video_id);
    //	audio_id=0;
    stmt.setInt("audio", audio_id);
    //	stmt.setInt("video",3);
    //	stmt.setInt("audio",0);
    cout << "VideoId" << video_id << "\taudio_id" << audio_id << endl;
    ResultSet rs = stmt.executeQuery();


    int video_packets = 0;
    int a = 0;
    int id = 0;
    int first_pts = -1;
    int first_dts = -1;
    /*
      AVRational ar2;
      ar2.num = 1;
      ar2.den = 25;
     */
    std::string path = org::esb::config::Config::getProperty("hive.base_path");
    path += "/tmp/";
    while (rs.next()) {
      int pu_id = rs.getInt("id");
      logdebug("open PU with id : " << pu_id)
      std::string name = path;
      name += org::esb::util::Decimal(pu_id % 10).toString();
      name += "/";
      name += org::esb::util::Decimal(pu_id).toString();
      name += ".unit";
      org::esb::io::File infile(name.c_str());
      if (!infile.exists()) {
        logerror(infile.getFilePath() << ": not found");
        continue;
      }
      org::esb::io::FileInputStream fis(&infile);
      org::esb::io::ObjectInputStream ois(&fis);
      org::esb::hive::job::ProcessUnit pu;
      ois.readObject(pu);
      //	  logdebug("here:"<<pu_id);
      //      logdebug(pu._process_unit);
      std::list<boost::shared_ptr<Packet> >::iterator plist = pu._output_packets.begin();
      for (; plist != pu._output_packets.end(); plist++) {
        Packet * p = (*plist).get();
        /*
                        if(p->getStreamIndex()==0)
                                p->setStreamIndex(1);
         */
        int idx = p->getStreamIndex();
        if (min_start_time > av_rescale_q(p->getPts(), p->getTimeBase(), basear))continue;

        p->setPts(p->getPts() - av_rescale_q(_source_stream_map[idx].in_start_time, _source_stream_map[idx].in_timebase, p->getTimeBase()));
        //        p->packet->dts=0;
        //        p->packet->pts=0;
        //		logdebug(p->toString());
        p->packet->stream_index = _source_stream_map[idx].out_stream_index;
        pos->writePacket(*p);

      }
      /*
      delete pu._decoder;
      pu._decoder = NULL;
      delete pu._encoder;
      pu._encoder = NULL;*/
      //	  infile.deleteFile();
    }
  }
  pos->close();
  fos->close();
  delete pos;
  delete fos;

  cout << endl;

  if (false) {
    Statement stmt = con.createStatement("select data_size, data, pts, dts, duration, flags, pos, stream_index from packets where stream_id=2 order by pts limit 10000");
    ResultSet rs = stmt.executeQuery();



    int audio_packets = 0;
    while (false && rs.next()) {
      //	    Row row=rs.getRow(a);
      audio_packets++;
      Packet p;
      p.packet->size = rs.getInt("data_size");
      p.packet->data = new uint8_t[p.packet->size];
      memcpy(p.packet->data, rs.getBlob("data").data(), p.packet->size);
      /*for some AudioStreams it might be pts=pts/duration */
      p.packet->pts = rs.getInt("pts"); //>0?(rs.getInt("pts")/rs.getInt("duration")):rs.getInt("pts");
      p.packet->dts = p.packet->pts; //rs.getint(3);
      p.packet->duration = rs.getInt("duration");
      p.packet->flags = rs.getInt("flags");
      p.packet->pos = rs.getInt("pos");
      p.packet->stream_index = 1; //rs.getint(7);
      pos->writePacket(p);
      if (audio_packets % 1000 == 0)
        cout << "\r" << audio_packets;
      cout.flush();

    }

    cout << "Audio Fertig:" << audio_packets << endl;
  }

  //  return 0;



}

FileExporter::FileExporter(void) {
}

FileExporter::~FileExporter(void) {
}
