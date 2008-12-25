#include "FileExporter.h"
#include "org/esb/sql/Connection.h"
#include "org/esb/sql/Statement.h"
#include "org/esb/sql/ResultSet.h"
#include "org/esb/av/PacketOutputStream.h"
#include "org/esb/av/Packet.h"
#include "org/esb/av/Decoder.h"
#include "org/esb/av/FormatOutputStream.h"
#include "org/esb/io/ObjectInputStream.h"
#include "org/esb/io/FileInputStream.h"
#include "org/esb/io/File.h"

#include "org/esb/config/config.h"
#include "org/esb/hive/CodecFactory.h"
#include "org/esb/hive/job/ProcessUnit.h"

#include <map>
using namespace org::esb::av;
using namespace org::esb::io;
using namespace org::esb::lang;
using namespace org::esb::sql;
using namespace org::esb::hive;
using namespace org::esb::config;


void FileExporter::exportFile(int fileid){
	

  map<int, Decoder*> enc;
  map<int, int> ptsmap;
  map<int, int> dtsmap;
  map<int, long long int> ptsoffset;
  map<int, long long int> dtsoffset;
//  map<int, int> dtsmap;
  std::string filename;
  Connection con(std::string(Config::getProperty("db.connection")));

  {
    PreparedStatement stmt = con.prepareStatement("select * from files where files.id=:id");
    stmt.setInt("id", fileid);
    ResultSet rs = stmt.executeQuery();
	if(rs.next()){
		if(rs.getString("path").size()==0){
			filename=Config::getProperty("hive.path");
		}else{
			filename=rs.getString("path");
		}
		filename+="/";
		filename+=rs.getString("filename");
	}
  }
  org::esb::io::File fout(filename.c_str());
  org::esb::io::File outDirectory(fout.getFilePath().c_str());
  if(!outDirectory.exists()){
	logdebug("creating output directory:"<<outDirectory.getFilePath());
    outDirectory.mkdir();
  }
//  string stream_id = fileid;
  FormatOutputStream fos(&fout);
  PacketOutputStream pos(&fos);

  int video_id = 0;
  int audio_id = 0;
  int audio_pts = 0;
  int video_pts = 0;
  bool build_offset=true;
  //    int v_num=0,v_den=0,a_num=0,a_den=0;

  {
    PreparedStatement stmt = con.prepareStatement("select *, streams.id as sid from files, streams where files.id=:id and streams.fileid=files.id and streams.id limit 2");
    stmt.setInt("id", fileid);
    ResultSet rs = stmt.executeQuery();

    while (rs.next()) {
      Decoder *codec=CodecFactory::getStreamDecoder(rs.getInt("sid"));
      codec->open();
//      Encoder *encoder=CodecFactory::getStreamEncoder(rs.getInt("sid"));
//      encoder->open();
      enc[rs.getInt("stream_index")]=codec;
      ptsmap[rs.getInt("stream_index")]=0;
      dtsmap[rs.getInt("stream_index")]=0;
      dtsoffset[rs.getInt("stream_index")]=-1;
      ptsoffset[rs.getInt("stream_index")]=-1;
      pos.setEncoder(*codec, rs.getInt("stream_index"));
      if (rs.getInt("stream_type") == CODEC_TYPE_VIDEO) {
        video_id = rs.getInt("sid");
      }

      if (rs.getInt("stream_type") == CODEC_TYPE_AUDIO) {
        audio_id = rs.getInt("sid");
      }
        if(codec->getCodecId()==CODEC_ID_MPEG2VIDEO){
          build_offset=false;
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
    }
    cout << "StreamTimeBaseNum:" << fos._fmtCtx->streams[rs.getInt("stream_index")]->time_base.num;
    cout << "\tStreamTimeBaseDen:" << fos._fmtCtx->streams[rs.getInt("stream_index")]->time_base.den;
    cout << "CodecTimeBaseNum:" << fos._fmtCtx->streams[rs.getInt("stream_index")]->codec->time_base.num;
    cout << "\tCodecTimeBaseDen:" << fos._fmtCtx->streams[rs.getInt("stream_index")]->codec->time_base.den;
    cout << endl;
  }
  //	fos.open();
  pos.init();

  {
    //    string sql="select a.data_size, a.data, a.pts, a.dts, a.duration, a.flags, a.pos, a.stream_index from packets a where a.stream_id=";
    //    sql+=stream_id;
    //    sql+=" order by a.pts limit 5000";
    //select * from packets where stream_id in(1,2) order by case when stream_id=1 then 1000/25000*pts else 1/16000*pts end;
    //select * from packets, streams s where stream_id=s.id and stream_id in (3,4)  order by s.time_base_num/s.time_base_den*pts
//    string sql = "select * from packets, streams s where stream_id=s.id and stream_id in (:video,:audio) order by s.time_base_num*dts/s.time_base_den";
	string sql = "SELECT * "
					"FROM process_units u, streams s "
					"WHERE u.target_stream = s.id "
					"AND s.id "
					"IN ( :video, :audio ) "
					"ORDER BY s.time_base_num * start_ts / s.time_base_den";
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
    int first_pts=-1;
    int first_dts=-1;
    /*
      AVRational ar2;
      ar2.num = 1;
      ar2.den = 25;
*/
	std::string path=org::esb::config::Config::getProperty("hive.path");
	path+="/tmp/";
    while (rs.next()) {
		std::string filename=path.append(rs.getString("u.id")).append(".unit");
		org::esb::io::File infile(filename.c_str());
		if(infile.exists()){
			org::esb::io::FileInputStream fis(&infile);
			org::esb::io::ObjectInputStream ois(&fis);
			org::esb::hive::job::ProcessUnit un;
			ois.readObject(un);
			list< boost::shared_ptr<Packet> >::iterator it;
			for (it = un._output_packets.begin(); it != un._output_packets.end(); it++) {
				boost::shared_ptr<Packet> p = *it;
				p->packet->pts = av_rescale_q(p->packet->pts, enc[p->getStreamIndex()]->getTimeBase(), fos._fmtCtx->streams[p->getStreamIndex()]->time_base);
				p->packet->dts = av_rescale_q(p->packet->dts, enc[p->getStreamIndex()]->getTimeBase(), fos._fmtCtx->streams[p->getStreamIndex()]->time_base);
			    if(build_offset){
					if(ptsoffset[p->getStreamIndex()]==-1){
						ptsoffset[p->getStreamIndex()]=p->packet->pts;
					}
					if(dtsoffset[p->getStreamIndex()]==-1){
						dtsoffset[p->getStreamIndex()]=p->packet->dts;
					}
					p->packet->pts=p->packet->pts-ptsoffset[p->getStreamIndex()];
					p->packet->dts=p->packet->dts-dtsoffset[p->getStreamIndex()];
				}
				pos.writePacket(*p);
			}
		}else{
			logerror("file not exist:"<<infile.getPath());
		}
      cout.flush();
    }
  }
}

FileExporter::FileExporter(void)
{
}

FileExporter::~FileExporter(void)
{
}
