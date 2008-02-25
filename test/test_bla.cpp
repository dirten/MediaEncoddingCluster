#include "org/esb/net/Socket.h"
#include "org/esb/io/File.h"
#include "org/esb/io/ObjectInputStream.h"
#include "org/esb/io/ObjectOutputStream.h"
#include "org/esb/io/FileOutputStream.h"
#include "org/esb/io/FileInputStream.h"
#include "org/esb/av/FormatInputStream.h"
#include "org/esb/av/FormatOutputStream.h"
#include "org/esb/av/AVInputStream.h"
#include "org/esb/av/PacketInputStream.h"
#include "org/esb/av/PacketOutputStream.h"
#include "org/esb/av/Packet.h"
#include "org/esb/av/Frame.h"
#include "org/esb/av/Encoder.h"
#include "org/esb/av/Decoder.h"
#include "org/esb/hive/job/ProcessUnit.h"
#include <iostream>
#include <stdexcept>
//#include "org/esb/sql/sqlite3x.hpp"
#include "org/esb/sql/Connection.h"
#include "org/esb/sql/ResultSet.h"
#include <boost/shared_ptr.hpp>
#include <avcodec.h>
//#include <sqlite3.h>
using namespace std;
using namespace org::esb;
using namespace org::esb::net;
using namespace org::esb::av;
using namespace org::esb::hive::job;
using namespace sqlite3x;

//void writePPM(Frame * frame, int width, int height);


void writePPM(Frame frame);
void encodeFromSocket();
void encodeFromProcessUnit();
void encodeFromFile();

int main(){
//    int ts=sqlite3::sqlite3_threadsafe();
//    encodeFromFile();
//    encodeFromSocket();
    encodeFromProcessUnit();
}
void encodeFromProcessUnit(){


    Socket sock("localhost", 20000);
    sock.connect();
    ObjectInputStream ois(sock.getInputStream());
    ObjectOutputStream oos(sock.getOutputStream());
    int size=0;

    File fout("/tmp/test.avi");
    FormatOutputStream fos(&fout);
    PacketOutputStream pos(&fos);


    
    Decoder * decoder=new Decoder(CODEC_ID_MSMPEG4V3);
    decoder->width=512;
    decoder->height=256;
    decoder->pix_fmt=PIX_FMT_YUV420P;
    decoder->setWidth(512);
    decoder->setHeight(256);
    decoder->setPixelFormat(PIX_FMT_YUV420P);
    decoder->open();


    CodecID cid=CODEC_ID_MSMPEG4V3;
//    CodecID cid=CODEC_ID_MPEG2VIDEO;

    Encoder *encoder=new Encoder(cid);
//    encoder->bit_rate=4000;
//    encoder->time_base=(AVRational){1,25};
//    encoder->gop_size=50;
//    encoder->pix_fmt=PIX_FMT_YUV420P;
//    encoder->mb_decision=20;
//    encoder->width=512;
//    encoder->height=256;

    encoder->setBitRate(400000);
    encoder->setTimeBase((AVRational){1,25});
//    encoder->gop_size=50;
    encoder->setGopSize(10);
    encoder->setPixelFormat(PIX_FMT_YUV420P);
//    encoder->mb_decision=20;
    encoder->setWidth(512);
    encoder->setHeight(256);
    encoder->open();
    pos.setEncoder(*encoder);




    size=0;
    int pCount=0;
    while(true||++pCount<20){
	char * text="get process_unit";
	sock.getOutputStream()->write(text, strlen(text));
//	cout << "hier"<<endl;
	ProcessUnit unit;
	ois.readObject(unit);
//	unit._decoder=decoder;
//	unit._decoder->open();
//	unit._encoder=encoder;
//	unit._encoder->open();
	
//	cout << "EncoderBitRate:"<<unit._encoder->bit_rate<<endl;
//	cout << "EncoderBitRate:"<<unit._encoder->gop_size<<endl;
	if(unit._input_packets.size()==0)break;
	unit.process();	

	char * text_out="put process_unit";
	sock.getOutputStream()->write(text_out, strlen(text_out));
	oos.writeObject(unit);
	
	list< boost::shared_ptr<Packet> >::iterator it; 
//	boost::shared_ptr<Packet> p=unit._input_packets.front();
//	p->size;
    
	for(it=unit._output_packets.begin();it!=unit._output_packets.end();it++){
	    boost::shared_ptr<Packet> p=*it;
//		cout << "Encoding size:"<<p->size<<endl;
//	    pos.writePacket(*p);
	}
	delete unit._decoder;
//	unit._encoder=encoder;
	delete unit._encoder;
//	if(++pCount>100)break;	
	cout << "\rDataSize="<<size/1024/1024;
	cout.flush();
    }


}


void encodeFromSocket(){


    Socket sock("localhost", 20000);
    sock.connect();
    ObjectInputStream ois(sock.getInputStream());
    int size=0;

    File fout("/tmp/test.vob");
    FormatOutputStream fos(&fout);
    PacketOutputStream pos(&fos);


    
    Decoder * decoder=new Decoder(CODEC_ID_MSMPEG4V3);
    decoder->width=512;
    decoder->height=256;
    decoder->pix_fmt=PIX_FMT_YUV420P;
    decoder->open();


    CodecID cid=CODEC_ID_MPEG2VIDEO;
//    CodecID cid=CODEC_ID_MSMPEG4V3;
    /*
    Decoder * decoder2=new Decoder(cid);
    decoder2->width=512;
    decoder2->height=256;
    decoder2->open(Codec::DECODER);
*/
    Encoder *encoder=new Encoder(cid);

    encoder->bit_rate=400000;
    encoder->time_base=(AVRational){1,25};
    encoder->gop_size=100;
    encoder->pix_fmt=PIX_FMT_YUV420P;
    encoder->mb_decision=20;
    encoder->width=512;
    encoder->height=256;
    encoder->open();
    pos.setEncoder(*encoder);



    size=0;
    int pCount=0;
    while(++pCount<20){
	char * text="get process_unit";
	sock.getOutputStream()->write(text, strlen(text));
//	cout << "hier"<<endl;
	ProcessUnit unit;
	ois.readObject(unit);
	list< boost::shared_ptr<Packet> >::iterator it; 
	if(unit._input_packets.size()==0)break;
//	boost::shared_ptr<Packet> p=unit._input_packets.front();
//	p->size;
	for(it=unit._input_packets.begin();it!=unit._input_packets.end();it++){
	    boost::shared_ptr<Packet> p=*it;
	    size+=p->size;
//	    cout << "new Frame size:"<<p->size<<endl;
	    cout << "PacketPts:"<<p->pts<<endl;
	    Frame f=decoder->decode(*p.get());
	    Packet ret=encoder->encode(f);
	    pos.writePacket(ret);
//	    delete ret;
//	    delete f3;
//	    delete f2;
//	    delete f;
	}
	
	cout << "\rDataSize="<<size/1024/1024;
	cout.flush();
    }
}


void encodeFromFile(){
//    File f("/tmp/Der Blutige Pfad Gottes - German (DVD-Quali).avi");
    File f("../Der Blutige Pfad Gottes - German (DVD-Quali).avi");
    File fout("/tmp/test.vob");
//    FileInputStream is(&f);
    FormatInputStream fis(&f);

    FormatOutputStream fos(&fout);
    PacketOutputStream pos(&fos);
//    AVInputStream * ais=fis.getAVStream(0);

//    Decoder *decoder=(Decoder*)ais->getCodec();



    Decoder * decoder=new Decoder(CODEC_ID_MSMPEG4V3);
    decoder->setWidth(512);
    decoder->setHeight(256);
    decoder->setPixelFormat(PIX_FMT_YUV420P);
    decoder->open();




    CodecID cid=CODEC_ID_MPEG2VIDEO;
//    CodecID cid=CODEC_ID_MSMPEG4V3;
//    CodecID cid=CODEC_ID_MPEG4;
    Decoder * decoder2=new Decoder(cid);
    decoder2->setWidth(512);
    decoder2->setHeight(256);
    decoder2->open();
    decoder2->pix_fmt=PIX_FMT_YUV420P;

    Encoder *encoder=new Encoder(cid);
    encoder->setBitRate(4000000);
    encoder->setTimeBase((AVRational){1,25});
    encoder->setGopSize(5);
//    encoder->max_b_frames=1;
    encoder->setPixelFormat(PIX_FMT_YUV420P);
//    encoder->pix_fmt=PIX_FMT_RGB24;
//    encoder->mb_decision=20;
    encoder->setWidth(decoder2->width);
    encoder->setHeight(decoder2->height);
//    encoder->flags|= CODEC_FLAG_GLOBAL_HEADER;
    encoder->open();

    Encoder *encoder2=new Encoder(cid);
    encoder2->bit_rate=4000000;
    encoder2->time_base=(AVRational){1,25};
    encoder2->gop_size=5;
    encoder2->pix_fmt=PIX_FMT_YUV420P;
    encoder2->width=decoder2->width;
    encoder2->height=decoder2->height;

    encoder2->setBitRate(4000000);
    encoder2->setTimeBase((AVRational){1,25});
    encoder2->setGopSize(5);
//    encoder->max_b_frames=1;
    encoder2->setPixelFormat(PIX_FMT_YUV420P);
//    encoder->pix_fmt=PIX_FMT_RGB24;
//    encoder->mb_decision=20;
    encoder2->setWidth(decoder2->width);
    encoder2->setHeight(decoder2->height);

    encoder2->open();
    pos.setEncoder(*encoder2);

//    int  test[10];
    PacketInputStream pis(&fis);
//    Packet p;
    while(true){
	Packet p=pis.readPacket();
	if(p.getStreamIndex()==0){
	    if(p.pts>=1){
		Frame f=decoder->decode(p);
		Packet p2=encoder->encode(f);
		pos.writePacket(p2);
//		cout<<"SizeA:"<<p.size<<" SizeB:"<<p2.size<<endl;
//		cout<<"Pts:"<<p2.pts<<" Dts:"<<p2.dts<<endl;
//		Frame f2=decoder2->decode(p2);
//		Frame tmp=f.getFrame(PIX_FMT_RGB24);
//		writePPM(f2.getFrame(PIX_FMT_RGB24));
//		writePPM(f2);
//		Packet * pac=encoder->encodeFrame(*f);
//		delete f;
//		delete f2;
//		delete p2;
//		if(p.pts>1050)
  //  		    break;
    	    }
	}
    }
    delete decoder;
    delete decoder2;
    delete encoder;


}
void writePPM(Frame  frame){
  FILE *pFile;
  char szFilename[32];
  int  y, width=frame.getWidth(), height=frame.getHeight();


    // Open file
  sprintf(szFilename, "frame%d.ppm", frame.pts);
  cout << "FileName:"<<szFilename<<endl;
  pFile=fopen(szFilename, "wb");
  if(pFile==NULL)
    return;

    // Write header
  fprintf(pFile, "P6\n%d %d\n255\n", width, height);

    // Write pixel data
  for(y=0; y<height; y++)
    fwrite(frame.data[0]+y*frame.linesize[0], 1, width*3, pFile);

    // Close file
  fclose(pFile);


}
