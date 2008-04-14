/*
#include "org/esb/io/File.h"
#include "org/esb/io/FileOutputStream.h"
#include "org/esb/io/ObjectOutputStream.h"
#include "org/esb/io/FileInputStream.h"
#include "org/esb/io/ObjectInputStream.h"
#include "org/esb/hive/job/ProcessUnit.h"
#include "org/esb/config/config.h"
#include "org/esb/av/FormatInputStream.h"
#include "org/esb/av/FormatOutputStream.h"
#include "org/esb/av/PacketInputStream.h"
#include "org/esb/av/PacketOutputStream.h"
#include "org/esb/av/Packet.h"
#include "org/esb/av/Frame.h"


#include <list>
#include "boost/shared_ptr.hpp"
using namespace org::esb::av;
using namespace org::esb::config;
using namespace org::esb::io;
using namespace org::esb::hive::job;

logger("main")

*/

//#include "org/esb/av/Decoder.h"

#include "avformat.h"
#include "avcodec.h"
#include <iostream>
using namespace std;
 
void testAV();
int main(){
    avcodec_init();

    /* register all the codecs */
    avcodec_register_all();

//    Config::init("./cluster.cfg");
//    const string log="log.properties";
//    log_init(cxxtools::Logger::LOG_LEVEL_DEBUG);

//    File file("/mnt/Video/sortiert/Der Blutige Pfad Gottes - German (DVD-Quali).avi");
//	FormatInputStream fois(&file);
//	fois.dumpFormat();

	testAV();

//    File file("/mnt/Video/sortiert/Der Blutige Pfad Gottes - German (DVD-Quali).avi");
//	FormatInputStream fois(&file);
//	fois.dumpFormat();
	
	
//	File file()
/*


	Decoder dec((CodecID)13);
	dec.setWidth(512);
	dec.setHeight(256);
	dec.setTimeBase((AVRational){1,25});
    dec.setGopSize(20);
    dec.setPixelFormat(PIX_FMT_YUV420P);

	dec.open();
	

	Encoder enc((CodecID)13);
	enc.setWidth(512);
	enc.setHeight(256);
	enc.setTimeBase((AVRational){1,25});
    enc.setGopSize(20);
    enc.setPixelFormat(PIX_FMT_YUV420P);
	enc.open();

	ProcessUnit unit;
//	unit._decoder=&dec;
//	unit._encoder=&enc;
	PacketInputStream pais(&fois);
//	Packet p;
//	list<boost::shared_ptr<Packet> > input_packets;

*/


/*
	for(int a=0;a<100;a++){
		pais.readPacket(p);
	
	
	}
*/
//           int len = avcodec_decode_video(c, picture, &got_picture, p.data, p.size);


//		Frame f=dec.decode(p);
//		Packet p2=enc.encode(f);
//		boost::shared_ptr<Packet> ptr(new Packet(p));
//		unit._input_packets.push_back(ptr);
	
//	unit.process();
/*
	FileOutputStream fos("test.unit");
	ObjectOutputStream oos(&fos);
	oos.writeObject(unit);
	fos.flush();
*/	
	
/*	
	ProcessUnit unit2;	
	FileInputStream fis("test.unit");
	ObjectInputStream ois(&fis);
	ois.readObject(unit2);

	unit2.process();

	delete unit2._decoder;
	delete unit2._encoder;
*/
//	delete unit2._decoder;
	
	
//	Config::close();
}


void testAV(){


	AVFormatContext *ic;
	
	
//    int err = av_open_input_file(&ic, "/mnt/Video/sortiert/Der Blutige Pfad Gottes - German (DVD-Quali).avi", NULL, 0, NULL);
    int err = av_open_input_file(&ic, "../dein_schuh.avi", NULL, 0, NULL);
//    int err = av_open_input_file(&ic, "../Der Blutige Pfad Gottes - German (DVD-Quali).avi", NULL, 0, NULL);
	if(err){
        fprintf(stderr, "could not open file\n");
	}
    err = av_find_stream_info(ic);


    AVCodec *codec;
    AVCodecContext *c = ic->streams[0]->codec;
   	codec = avcodec_find_decoder(CODEC_ID_MPEG4);
//    c= avcodec_alloc_context();
    AVFrame* picture= avcodec_alloc_frame();
	if (avcodec_open(c, codec) < 0) {
        fprintf(stderr, "could not open codec\n");
        exit(1);
    }
//	int ret = av_read_frame(ic, pkt);


    AVPacket  *pkt;

	int got_picture;
	for(int i=0;i<100;i++){
		int ret = av_read_frame(ic, pkt);
		
//		pais.readPacket(p);
		if(pkt->stream_index!=0)continue;

    	int bytesRemaining=pkt->size,  bytesDecoded=0;
    	uint8_t * rawData=pkt->data;
    	while(bytesRemaining > 0){
			bytesDecoded=avcodec_decode_video(c, picture, &got_picture, rawData, bytesRemaining);
			if(bytesDecoded < 0){
	    		fprintf(stderr, "Error while decoding frame\n");
	    		break;
			}
			bytesRemaining-=bytesDecoded;
			rawData+=bytesDecoded;
			if(got_picture){
				cout << "got Picture"<<endl;
	    		break;
			}
    	}
	}


	avcodec_close(c);


}


