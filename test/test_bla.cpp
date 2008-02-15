#include "org/esb/net/Socket.h"
#include "org/esb/io/File.h"
#include "org/esb/io/ObjectInputStream.h"
#include "org/esb/io/ObjectOutputStream.h"
#include "org/esb/io/FileOutputStream.h"
#include "org/esb/io/FileInputStream.h"
#include "org/esb/av/FormatInputStream.h"
#include "org/esb/av/AVInputStream.h"
#include "org/esb/av/PacketInputStream.h"
#include "org/esb/av/Packet.h"
#include "org/esb/av/Frame.h"
#include "org/esb/av/Encoder.h"
#include "org/esb/av/Decoder.h"
#include "org/esb/hive/job/ProcessUnit.h"
#include <iostream>
#include <stdexcept>
#include "org/esb/sql/sqlite3x.hpp"
#include "org/esb/sql/Connection.h"
#include "org/esb/sql/ResultSet.h"
#include <boost/shared_ptr.hpp>
#include <avcodec.h>
using namespace std;
using namespace org::esb;
using namespace org::esb::net;
using namespace org::esb::av;
using namespace org::esb::hive::job;
using namespace sqlite3x;

//void writePPM(Frame * frame, int width, int height);


void writePPM(Frame frame);
int main(){


    Socket sock("localhost", 20000);
    sock.connect();
    ObjectInputStream ois(sock.getInputStream());
    int size=0;
    File f("/tmp/Der Blutige Pfad Gottes - German (DVD-Quali).avi");
//    FileInputStream is(&f);
    FormatInputStream fis(&f);
//    AVInputStream * ais=fis.getAVStream(0);

//    Decoder *decoder=(Decoder*)ais->getCodec();



    Decoder * decoder=new Decoder(CODEC_ID_MSMPEG4V3);
    decoder->width=512;
    decoder->height=256;
    decoder->pix_fmt=PIX_FMT_YUV420P;

/*
    cout << "CodecId MPEG:"<<CODEC_ID_MPEG2VIDEO<<endl;
    Codec *encoder=new Codec(CODEC_ID_MPEG2VIDEO,2);
    encoder->width=decoder->width;
    encoder->height=decoder->height;
    encoder->flags|= CODEC_FLAG_GLOBAL_HEADER;
    encoder->open(Codec::ENCODER);
//    encoder->initDefaults();
    cout << "width:"<<encoder->coded_width<<endl;
    cout << "height:"<<encoder->coded_height<<endl;
    cout << "width:"<<encoder->width<<endl;
    cout << "height:"<<encoder->height<<endl;
*/
    

//    Codec *codec=ais->getCodec();
//    codec->open(Codec::DECODER);
//    cout << "CodecInit:"<<decoder->codec->id<<endl;
    decoder->open(Codec::DECODER);




    CodecID cid=CODEC_ID_MPEG2VIDEO;
//    CodecID cid=CODEC_ID_MSMPEG4V3;
//    CodecID cid=CODEC_ID_MPEG4;
    Decoder * decoder2=new Decoder(cid);
    decoder2->width=512;
    decoder2->height=256;
    decoder2->open(Codec::DECODER);
    decoder2->pix_fmt=PIX_FMT_YUV420P;

    Encoder *encoder=new Encoder(cid);

    encoder->bit_rate=400000;
    encoder->time_base=(AVRational){1,25};
    encoder->gop_size=10;
//    encoder->max_b_frames=1;
    encoder->pix_fmt=PIX_FMT_YUV420P;
//    encoder->pix_fmt=PIX_FMT_RGB24;
//    encoder->mb_decision=20;


    encoder->width=decoder2->width;
    encoder->height=decoder2->height;
//    encoder->flags|= CODEC_FLAG_GLOBAL_HEADER;
    encoder->open(Codec::ENCODER);


//    int  test[10];
    PacketInputStream pis(&fis);
//    Packet p;
    while(true){
	Packet p=pis.readPacket();
	if(p.getStreamIndex()==0){
	    if(p.pts>=100){
		Frame f=decoder->decode(p);
		Packet p2=encoder->encode(f);
		cout<<"SizeA:"<<p.size<<" SizeB:"<<p2.size<<endl;
		cout<<"Pts:"<<p2.pts<<" Dts:"<<p2.dts<<endl;
		Frame f2=decoder2->decode(p2);
//		Frame tmp=f.getFrame(PIX_FMT_RGB24);
		writePPM(f2.getFrame(PIX_FMT_RGB24));
//		writePPM(f2);
//		Packet * pac=encoder->encodeFrame(*f);
//		delete f;
//		delete f2;
//		delete p2;
		if(p.pts>150)
    		    break;
    	    }
	}
    }
    delete decoder;
    delete decoder2;
    delete encoder;
/*
return 0;

//    CodecID cid=CODEC_ID_MPEG1VIDEO;
    CodecID cid=CODEC_ID_MSMPEG4V3;
    Decoder * decoder2=new Decoder(cid);
    decoder2->width=512;
    decoder2->height=256;
    decoder2->open(Codec::DECODER);

    Encoder *encoder=new Encoder(cid);

    encoder->bit_rate=400000;
    encoder->time_base=(AVRational){1,25};
    encoder->gop_size=10;
//    encoder->max_b_frames=2;
    encoder->pix_fmt=PIX_FMT_YUV420P;
    encoder->mb_decision=2;


    encoder->width=decoder2->width;
    encoder->height=decoder2->height;
//    encoder->flags|= CODEC_FLAG_GLOBAL_HEADER;
    encoder->open(Codec::ENCODER);

*/
/*
    AVFrame*picture;
	    picture=avcodec_alloc_frame();

	    int outbuf_size = 100000;
	    uint8_t * outbuf = new uint8_t[outbuf_size];
	    size = decoder->width * decoder->height;
	    uint8_t * picture_buf = new uint8_t[((size * 3) / 2)]; 

	    picture->data[0] = picture_buf;
            picture->data[1] = picture->data[0] + size;
	    picture->data[2] = picture->data[1] + size / 4;
	    picture->linesize[0] = decoder->width;
	    picture->linesize[1] = decoder->width / 2;
	    picture->linesize[2] = decoder->width / 2;
	    encoder->encode((Frame&)*picture);
*/
/*
    size=0;

    while(true){
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
	    Frame * f=decoder->decode(*p.get());

//	    writePPM(f->getFrame(PIX_FMT_RGB24));
//	    Packet * pac=encoder->encodeFrame(*f);

	    Packet * ret=encoder->encode(*f);

	    Frame * f2 = decoder2->decode(*ret);

	    Frame *f3= f2->getFrame(PIX_FMT_RGB24);
//	    writePPM(f2->getFrame(PIX_FMT_RGB24));
	    writePPM(f3);



	    delete ret;
	    delete f3;
	    delete f2;
	    delete f;
	}
	
	cout << "\rDataSize="<<size/1024/1024;
	cout.flush();
    }*/
    
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
