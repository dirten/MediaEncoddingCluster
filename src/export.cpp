
#include "org/esb/sql/Connection.h"
#include "org/esb/sql/Statement.h"
#include "org/esb/sql/ResultSet.h"
#include "org/esb/av/PacketOutputStream.h"
#include "org/esb/av/Packet.h"
#include "org/esb/av/Decoder.h"
#include "org/esb/av/FormatOutputStream.h"
#include "org/esb/io/File.h"
#include "org/esb/hive/CodecFactory.h"

#include <map>
using namespace org::esb::av;
using namespace org::esb::io;
using namespace org::esb::lang;
using namespace org::esb::sql;
using namespace org::esb::hive;

int exporter(char * fileid, char * outfile) {

  map<int, Decoder*> enc;
  map<int, int> ptsmap;
  map<int, int> dtsmap;
  map<int, long long int> ptsoffset;
  map<int, long long int> dtsoffset;
//  map<int, int> dtsmap;
  org::esb::io::File fout(outfile);
  string stream_id = fileid;
  FormatOutputStream fos(&fout);
  PacketOutputStream pos(&fos);

  Connection con(std::string(Config::getProperty("db.connection")));
//  Encoder *encoder = NULL;
  int video_id = 0;
  int audio_id = 0;
  int audio_pts = 0;
  int video_pts = 0;
  bool build_offset=true;
  //    int v_num=0,v_den=0,a_num=0,a_den=0;

  {
    PreparedStatement stmt = con.prepareStatement("select *, streams.id as sid from files, streams where files.id=:id and streams.fileid=files.id and streams.id limit 2");
    stmt.setString("id", stream_id);
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
    string sql = "select * from packets, streams s where stream_id=s.id and stream_id in (:video,:audio) order by s.time_base_num*dts/s.time_base_den";
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
    while (rs.next()) {
      //	    Row row=rs.getRow(a);
      //	    video_packets++;
      //	    cout<<"" << rs.getInt("id")<<endl;
      Packet p(rs.getInt("data_size"));
      p.packet->stream_index = rs.getInt("stream_index");
      p.packet->flags = rs.getInt("flags");
//      p.packet->size = rs.getInt("data_size");
      p.packet->duration=1;
      //	    p.size=rs.getInt("data_size");
      //	    p.data=new uint8_t[p.size];
      //		if(p.packet->stream_index==0){
      //        if(p.packet->stream_index==CODEC_TYPE_VIDEO)
      p.packet->dts = AV_NOPTS_VALUE; //rs.getInt("dts");
      p.packet->pts = AV_NOPTS_VALUE; //rs.getInt("dts");

      AVRational ar2={1,25};//enc[rs.getInt("stream_index")]->getTimeBase();
//      if(enc[rs.getInt("stream_index")]->getCodecType()==CODEC_TYPE_VIDEO){

        p.packet->pts = av_rescale_q(rs.getLong("pts"), enc[rs.getInt("stream_index")]->getTimeBase(), fos._fmtCtx->streams[p.packet->stream_index]->time_base);
        p.packet->dts = av_rescale_q(rs.getLong("dts"), enc[rs.getInt("stream_index")]->getTimeBase(), fos._fmtCtx->streams[p.packet->stream_index]->time_base);
//      }


        if(build_offset){
            if(ptsoffset[rs.getInt("stream_index")]==-1){
              ptsoffset[rs.getInt("stream_index")]=p.packet->pts;
            }
            if(dtsoffset[rs.getInt("stream_index")]==-1){
            dtsoffset[rs.getInt("stream_index")]=p.packet->dts;
            }
            p.packet->pts=p.packet->pts-ptsoffset[rs.getInt("stream_index")];
            p.packet->dts=p.packet->dts-dtsoffset[rs.getInt("stream_index")];
        }

        if(false&&enc[rs.getInt("stream_index")]->getCodecType()==CODEC_TYPE_VIDEO){
            p.packet->pts = av_rescale_q(rs.getLong("pts"), enc[rs.getInt("stream_index")]->getTimeBase(), ar2);
            p.packet->dts = av_rescale_q(rs.getLong("dts"), enc[rs.getInt("stream_index")]->getTimeBase(), ar2);
      }
//        printf("PTS=%lli DTS=%lli Stream=%i\n",p.packet->pts,p.packet->dts,rs.getInt("stream_index"));
//        cout << "PTS="<<p.packet->pts<<endl;
        if(false){
            p.packet->pts =++ptsmap[rs.getInt("stream_index")];
            p.packet->dts =++dtsmap[rs.getInt("stream_index")];
        }
      //          p.packet->pts=av_rescale_q(++video_packets,fos._fmtCtx->streams[p.packet->stream_index]->codec->time_base,fos._fmtCtx->streams[p.packet->stream_index]->time_base);



      //    	p.packet->pts=(rs.getDouble("pts")-rs.getDouble("start_time"));///rs.getInt("duration");
      //			p.packet->duration=1;//rs.getInt("duration");
      //        if(p.packet->stream_index==CODEC_TYPE_AUDIO)
      //          p.packet->pts=AV_NOPTS_VALUE;
      /*
              cout << "\tPacketIndex"<<p.packet->stream_index;
              cout << "\tPacketPts"<<p.packet->pts;
              cout << "\tPacketDuration"<<p.packet->duration;
            cout << endl;
       */
      /*
                  }else
                  if(p.packet->stream_index==1){
                      p.packet->pts=audio_pts;//AV_NOPTS_VALUE;//rs.getInt("pts");
                      audio_pts+=rs.getInt("duration");
                      p.packet->duration=384;//rs.getInt("duration");
                  }
       */

//      p.packet->pts = rs.getLong("pts");
//      p.packet->dts = rs.getLong("dts");
      
//      p.packet->dts = dtsmap[rs.getInt("stream_index")];//rs.getLong("dts");
//      dtsmap[rs.getInt("stream_index")]+=rs.getInt("duration");
      //	    p.packet->pts=rs.getDouble("pts")>0?(rs.getDouble("pts")/rs.getDouble("duration")):rs.getDouble("pts");
      //        if(rs.getInt("stream_type")==CODEC_TYPE_VIDEO){
      //	      p.packet->dts=rs.getDouble("dts")>0?(rs.getDouble("dts")/rs.getDouble("duration")):rs.getDouble("dts");
      //	    }
      //	    p.packet->dts=rs.getInt("dts");
      //	    p.packet->pos=0;//rs.getInt("pos");
//        p.packet->data=static_cast<unsigned int*>(const_cast<char*>(rs.getBlob("data").data()));
      memcpy(p.packet->data, rs.getBlob("data").data(), p.packet->size);
      //		cout << "PacketSize:"<<p.packet->size<<"="<<rs.getBlob("data").length()<<endl;
      pos.writePacket(p);
      //	    if(video_packets%1000==0)
      cout.flush();
    }
  }

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
      p.packet->duration = 1; //rs.getInt("duration");
      p.packet->flags = rs.getInt("flags");
      p.packet->pos = rs.getInt("pos");
      p.packet->stream_index = 1; //rs.getint(7);
      pos.writePacket(p);
      if (audio_packets % 1000 == 0)
        cout << "\r" << audio_packets;
      cout.flush();

    }

    cout << "Audio Fertig:" << audio_packets << endl;
  }

  return 0;
}


