

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
using namespace org::esb::sql;
using namespace org::esb::lang;

int exporter(int argc, char * argv[]){

    File fout("/tmp/testdb.avi");
//    File fout("/tmp/testdb.mp2");
    FormatOutputStream fos(&fout);
    PacketOutputStream pos(&fos);

    Encoder *encoder=new Encoder(CODEC_ID_MSMPEG4V3);
    encoder->setBitRate(4000000);
    encoder->setTimeBase((AVRational){1,25});
    encoder->setGopSize(10);
    encoder->setPixelFormat(PIX_FMT_YUV420P);
    encoder->setWidth(512);
    encoder->setHeight(256);
    encoder->open();

    pos.setEncoder(*encoder,0);

    Encoder *encoder2=new Encoder(CODEC_ID_MP2);
    encoder2->setBitRate(128000);
    encoder2->setSampleRate(44100);
    encoder2->setChannels(2);
    encoder2->setSampleFormat((SampleFormat)1);
    encoder2->open();

    pos.setEncoder(*encoder2,1);

/*
    Decoder *decoder2=new Decoder(CODEC_ID_MP3);
    decoder2->setBitRate(128000);
    decoder2->setSampleRate(44100);
    decoder2->setChannels(2);
    decoder2->setSampleFormat(1);
    decoder2->open();
    pos.setEncoder((Encoder&)*decoder2,0);
*/
    pos.init();
//    if(false)
	string dbFile=Config::getProperty("data.dir");
	dbFile+="/";
	dbFile+=Config::getProperty("data.file");
	Connection con((char*)dbFile.c_str());

    {
	Statement stmt=con.createStatement("select a.data_size, a.data, a.pts, a.dts, a.duration, a.flags, a.pos, a.stream_index from packets a where a.stream_id=1 order by a.pts limit 10000");
	ResultSet rs=stmt.executeQuery();
	
	

	int video_packets=0;

	while(rs.next()){
	    video_packets++;
	    Packet p;
	    p.size=rs.getInt("data_size");
	    p.data=new uint8_t[p.size];
	    memcpy(p.data,rs.getBlob("data").c_str(),p.size);
	    p.pts=rs.getInt("pts");
	    p.dts=rs.getInt("dts");
	    p.duration=rs.getInt("duration");
	    p.flags=rs.getInt("flags");
	    p.pos=rs.getInt("pos");
	    p.stream_index=0;//rs.getint(7);
	    pos.writePacket(p);
		if(video_packets%1000==0)
	    cout<<"\r" << video_packets ;
	    cout.flush();

	}
    }
    
	    cout<<endl;
    
        {
	Statement stmt=con.createStatement("select data_size, data, pts, dts, duration, flags, pos, stream_index from packets where stream_id=2 order by pts limit 10000");
	ResultSet rs=stmt.executeQuery();
	
	

	int audio_packets=0;
	while(rs.next()){
	    audio_packets++;
	    Packet p;
	    p.size=rs.getInt("data_size");
	    p.data=new uint8_t[p.size];
	    memcpy(p.data,rs.getBlob("data").c_str(),p.size);
	    /*for some AudioStreams it might be pts=pts/duration */
	    p.pts=rs.getInt("pts")>0?(rs.getInt("pts")/rs.getInt("duration")):rs.getInt("pts");
	    p.dts=p.pts;//rs.getint(3);
	    p.duration=1;//rs.getInt("duration");
	    p.flags=rs.getInt("flags");
	    p.pos=rs.getInt("pos");
	    p.stream_index=1;//rs.getint(7);
	    pos.writePacket(p);
		if(audio_packets%1000==0)
	    cout<<"\r" << audio_packets ;
	    cout.flush();

	}

	cout << "Audio Fertig:"<<audio_packets<<endl;
    }

return 0;
}


