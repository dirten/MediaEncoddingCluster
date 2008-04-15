
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




#include "org/esb/av/Decoder.h"
#include "avformat.h"
#include "avcodec.h"
#include <iostream>
using namespace std;
 
//void testAV();
int main(int argc, char ** argv){
    av_register_all();

    avcodec_init();
    avcodec_register_all();

    char * filename=argv[1];

/*
    AVFormatContext *ic;
	
//    File file("spongebob.schwammkopf.-.s02e01_1.-.hey.dein.schuh.ist.offen.by.dreitausend1.avi");
//	FormatInputStream fois(&file);
//	fois.dumpFormat();
//	ic= av_alloc_format_context();
//    int err = av_open_input_file(&ic, "/mnt/Video/sortiert/Der Blutige Pfad Gottes - German (DVD-Quali).avi", NULL, 0, NULL);
    int err = av_open_input_file(&ic, filename, NULL, 0, NULL);
//    int err = av_open_input_file(&ic, "test.dvd", NULL, 0, NULL);
//    int err = av_open_input_file(&ic, "../dein_schuh.avi", NULL, 0, NULL);
//    int err = av_open_input_file(&ic, "../Der Blutige Pfad Gottes - German (DVD-Quali).avi", NULL, 0, NULL);
	if(err){
        fprintf(stderr, "could not open file\n");
	}
    err = av_find_stream_info(ic);
//    dump_format(ic, 0, "spongebob.schwammkopf.-.s02e01_1.-.hey.dein.schuh.ist.offen.by.dreitausend1.avi", 0);

    AVCodec *codec;
    AVCodecContext *c = ic->streams[0]->codec;
    codec = avcodec_find_decoder(CODEC_ID_MPEG4);
//    AVCodecContext *c= avcodec_alloc_context();
    AVFrame picture;//= avcodec_alloc_frame();
    avcodec_get_frame_defaults(&picture);
	if (avcodec_open(c, codec) < 0) {
        fprintf(stderr, "could not open codec\n");
        exit(1);
    }
    */
//	int ret = av_read_frame(ic, pkt);


    File file(filename);
    FormatInputStream fois(&file);
    AVFormatContext *ic=fois.getFormatContext();

    fois.dumpFormat();
    PacketInputStream pis(fois);
//    cout << "Width"<<ic->streams[0]->codec->width<<endl;
	Decoder dec(ic->streams[0]->codec->codec_id);
	dec.setWidth(ic->streams[0]->codec->width);
	dec.setHeight(ic->streams[0]->codec->height);
	dec.setPixelFormat(ic->streams[0]->codec->pix_fmt);
	dec.open();

	Encoder enc(CODEC_ID_MPEG2VIDEO);
	enc.setWidth(ic->streams[0]->codec->width);
	enc.setHeight(ic->streams[0]->codec->height);
	enc.setTimeBase((AVRational){1,25});
	enc.setGopSize(20);
	enc.setPixelFormat(PIX_FMT_YUV420P);
	enc.open();

//    AVPacket pkt;
	int got_picture;
	for(int i=0;i<30;i++){
//	    memset(p.packet,0,sizeof(AVPacket));
	    Packet p;
	    pis.readPacket(p);
//	    int ret = av_read_frame(ic, p.packet);
//	    pkt=*p.packet;
		
//		pais.readPacket(p);
    	    if(p.packet->stream_index!=0)continue;

//    	int bytesRemaining=pkt.size,  bytesDecoded=0;
//    	uint8_t * rawData=pkt.data;
//    	while(bytesRemaining > 0){
	    AVFrame * picture= avcodec_alloc_frame();

    	    cout << "Loop"<<endl;
	    Frame f = dec.decode(p);
	    cout <<"FrameHere"<<endl;
	    enc.encode(f);


	av_free(picture);

	/*
	int bytesDecoded=avcodec_decode_video(c, &picture, &got_picture, p.packet->data, p.packet->size);
		if(bytesDecoded < 0){
        		fprintf(stderr, "Error while decoding frame\n");
        		break;
		}
	*/
//    	}
//    	av_free_packet(p.packet);
	}
//    	av_free_packet(p.packet);

//	delete []rawData;


//	av_free(&picture);
//	avcodec_close(c);
//	av_close_input_file(ic);
//	av_free(ic);

//    Config::init("./cluster.cfg");
//    const string log="log.properties";
//    log_init(cxxtools::Logger::LOG_LEVEL_DEBUG);

//    File file("/mnt/Video/sortiert/Der Blutige Pfad Gottes - German (DVD-Quali).avi");
//	FormatInputStream fois(&file);
//	fois.dumpFormat();

//	testAV();

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

