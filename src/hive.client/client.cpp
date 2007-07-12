#include "org/esb/net/Socket.h"
#include "org/esb/io/InputStream.h"
#include "org/esb/io/OutputStream.h"
#include "org/esb/av/FrameOutputStream.h"
#include "org/esb/av/PacketOutputStream.h"
#include "org/esb/av/PacketInputStream.h"
#include "org/esb/av/CodecInputStream.h"
#include "org/esb/io/FileOutputStream.h"
#include "org/esb/av/Frame.h"
#include <iostream>
#include "avformat.h"
using namespace std;
using namespace org::esb::net;
using namespace org::esb::io;
using namespace org::esb::av;
#define STREAM_FRAME_RATE 25 /* 25 images/s */
//#define STREAM_NB_FRAMES  ((int)(STREAM_DURATION * STREAM_FRAME_RATE))
#define STREAM_PIX_FMT PIX_FMT_YUV420P /* default pix_fmt */

int main(int argc,char**argv){
    Socket * socket=new Socket("localhost",20000);
    socket->connect();
    InputStream *input=socket->getInputStream();
    if(input == NULL){
	cout << "Kein inputstream"<<endl;
	exit(1);
    }	
    OutputStream *output=socket->getOutputStream();
    if(output == NULL){
	cout << "Kein outputstream"<<endl;
	exit(1);
    }
    
    PacketInputStream pis(input);
    CodecInputStream codecis(input);
    PacketOutputStream pos(output);
    
    int counter=0;
    int a=0;
     av_register_all();


    string getcodec="get inputcodec";
    output->write((char*)getcodec.c_str(),getcodec.size());

    Codec * codec=codecis.readCodec();
	codec->open();
	string getcmd="get frame";
	string putcmd="put frame";
	unsigned char * test=new unsigned char[10];

	Codec outCodec(CODEC_ID_MPEG4);

	AVCodecContext *cc=avcodec_alloc_context();
    cc->codec_type = CODEC_TYPE_VIDEO;
	cc->height=codec->getCodecContext()->height;
	cc->width=codec->getCodecContext()->width;
	cc->bit_rate=200000;
	cc->max_b_frames=2;
    cc->time_base.den = STREAM_FRAME_RATE;
    cc->time_base.num = 1;
    cc->gop_size = 40; /* emit one intra frame every twelve frames at most */
    cc->pix_fmt = STREAM_PIX_FMT;
//    cc->flags |= CODEC_FLAG_GLOBAL_HEADER;

	AVCodec * c=avcodec_find_encoder(CODEC_ID_MPEG4);
    if (!c) {
        fprintf(stderr, "codec not found\n");
        exit(1);
    }
	cc->codec_id=c->id;
	
    if (avcodec_open(cc, c) < 0) {
        fprintf(stderr, "could not open codec\n");
        exit(1);
    }

//	outCodec.open();
	
    while(!socket->isClosed()){
//	int size=input->read(test,10);
	output->write((char*)getcmd.c_str(),getcmd.size());

	Packet packet=pis.readPacket();
	if(a>=30000)exit(0);
	if(a%1000==0)
	cout << a<<"Packet size:"<<packet.getSize()<<endl;
		
	Frame frame(&packet, codec);
	uint8_t * video_outbuf=NULL;
	int video_outbuf_size;
    video_outbuf_size = 400000;
    video_outbuf = (uint8_t*)av_malloc(video_outbuf_size);


//    Frame* rgb=frame.getFrame(PIX_FMT_RGB24);

    int out_size = avcodec_encode_video(cc, video_outbuf, video_outbuf_size, &frame);
	delete video_outbuf;
//	cout << counter<<"neues Frame size:"<<frame.getSize()<<endl;

//        Frame* rgb=frame.getFrame(PIX_FMT_RGB24);

/*
        char filename[32];
        sprintf(filename,"/tmp/hive/test.%d.ppm",a);

        FileOutputStream *out=new FileOutputStream(filename);
        FrameOutputStream *fout=new FrameOutputStream(out);

        char header[200];
        sprintf(header, "P6\n%d %d\n255\n", rgb->getWidth(), rgb->getHeight());
        fout->write(header, strlen(header));
        fout->writeFrame(rgb);
        delete fout;
        delete out;
        
*/

  //      delete rgb;


        a++;



    }
}



