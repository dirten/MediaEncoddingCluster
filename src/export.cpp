
#include "org/esb/sql/Connection.h"
#include "org/esb/sql/Statement.h"
#include "org/esb/sql/ResultSet.h"
#include "org/esb/av/PacketOutputStream.h"
#include "org/esb/av/Packet.h"
#include "org/esb/av/Decoder.h"
#include "org/esb/av/FormatOutputStream.h"
#include "org/esb/lang/Thread.h"
#include "org/esb/io/File.h"

using namespace org::esb::av;
using namespace org::esb::io;
using namespace org::esb::lang;
using namespace org::esb::sql;

int exporter(int argc, char * argv[]){

    File fout("/tmp/testdb.avi");
//    File fout("/tmp/testdb.mp2");
    string stream_id=argv[2];
//    int codec_id=atoi(argv[3]);
    FormatOutputStream fos(&fout);
    PacketOutputStream pos(&fos);

	Connection con(Config::getProperty("db.connection"));
    Decoder *encoder=NULL;
    int video_id=0;
    int audio_id=0;

  {
    PreparedStatement stmt=con.prepareStatement("select *, streams.id as sid from files, streams where files.id=:id and streams.fileid=files.id");
    stmt.setString("id", stream_id);
    ResultSet rs=stmt.executeQuery();
    
    while(rs.next()){
      encoder=new Decoder((CodecID)rs.getInt("codec"));
      encoder->setBitRate(rs.getInt("bit_rate"));
      encoder->setTimeBase((AVRational){rs.getInt("time_base_num"),rs.getInt("time_base_den")});
      encoder->setGopSize(rs.getInt("gop_size"));
      encoder->setPixelFormat((PixelFormat)rs.getInt("pix_fmt"));
      encoder->setWidth(rs.getInt("width"));
      encoder->setHeight(rs.getInt("height"));
      encoder->setSampleRate(rs.getInt("sample_rate"));
      encoder->setChannels(rs.getInt("channels"));
      encoder->setSampleFormat((SampleFormat)rs.getInt("sample_fmt"));
      encoder->open();
      if(encoder->ctx->codec_type==CODEC_TYPE_VIDEO){
        video_id=rs.getInt("sid");
        cout <<"VideoId"<<video_id<<"\taudio_id"<<audio_id<<endl;
        pos.setEncoder(*encoder,rs.getInt("stream_index"));

      }
      if(encoder->ctx->codec_type==CODEC_TYPE_AUDIO){
        audio_id=rs.getInt("sid");
        cout <<"VideoId"<<video_id<<"\taudio_id"<<audio_id<<endl;
        cout << "Num:"<<encoder->ctx->time_base.num<<"\tDen:"<<encoder->ctx->time_base.den<<endl;
        pos.setEncoder(*encoder,rs.getInt("stream_index"));
      }
      
    }
  }
    pos.init();


    {
//    string sql="select a.data_size, a.data, a.pts, a.dts, a.duration, a.flags, a.pos, a.stream_index from packets a where a.stream_id=";
//    sql+=stream_id;
//    sql+=" order by a.pts limit 5000";
//select * from packets where stream_id in(1,2) order by case when stream_id=1 then 1000/25000*pts else 1/16000*pts end;
    string sql="select * from packets where stream_id in (:video, :audio) order by pos/* order by case when stream_index = 0  then 1/25*pts else 3/125*pts end*/";
	PreparedStatement stmt=con.prepareStatement(sql.c_str());
	stmt.setInt("video",video_id);
	stmt.setInt("audio",audio_id);
    cout <<"VideoId"<<video_id<<"\taudio_id"<<audio_id<<endl;
	ResultSet rs=stmt.executeQuery();
	

	int video_packets=0;

	while(rs.next()){
//	    Row row=rs.getRow(a);
	    video_packets++;
	    cout<<"\r" << rs.getInt("id");
	    Packet p(rs.getInt("data_size"));
//	    p.size=rs.getInt("data_size");
//	    p.data=new uint8_t[p.size];
//	    p.packet->pts=AV_NOPTS_VALUE;//rs.getInt("pts");
//	    p.packet->dts=AV_NOPTS_VALUE;//rs.getInt("dts");
	    p.packet->pts=rs.getInt("pts");
	    p.packet->dts=rs.getInt("dts");
	    p.packet->duration=rs.getInt("duration");
	    p.packet->flags=rs.getInt("flags");
	    p.packet->pos=rs.getInt("pos");
	    p.packet->stream_index=rs.getInt("stream_index");
	    memcpy(p.packet->data,rs.getBlob("data").data(),p.packet->size);

	    pos.writePacket(p);
//	    if(video_packets%1000==0)
	    cout.flush();
	}
    }
    
	    cout<<endl;
    
        if(false){
	Statement stmt=con.createStatement("select data_size, data, pts, dts, duration, flags, pos, stream_index from packets where stream_id=2 order by pts limit 10000");
	ResultSet rs=stmt.executeQuery();
	
	

	int audio_packets=0;
	while(false&&rs.next()){
//	    Row row=rs.getRow(a);
	    audio_packets++;
	    Packet p;
	    p.packet->size=rs.getInt("data_size");
	    p.packet->data=new uint8_t[p.packet->size];
	    memcpy(p.packet->data,rs.getBlob("data").data(),p.packet->size);
	    /*for some AudioStreams it might be pts=pts/duration */
	    p.packet->pts=rs.getInt("pts")>0?(rs.getInt("pts")/rs.getInt("duration")):rs.getInt("pts");
	    p.packet->dts=p.packet->pts;//rs.getint(3);
	    p.packet->duration=1;//rs.getInt("duration");
	    p.packet->flags=rs.getInt("flags");
	    p.packet->pos=rs.getInt("pos");
	    p.packet->stream_index=1;//rs.getint(7);
	    pos.writePacket(p);
		if(audio_packets%1000==0)
	    cout<<"\r" << audio_packets ;
	    cout.flush();

	}

	cout << "Audio Fertig:"<<audio_packets<<endl;
    }

return 0;
}


