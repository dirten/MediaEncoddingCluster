#include "org/esb/io/File.h"
#include "org/esb/av/FormatInputStream.h"
#include "org/esb/av/FormatOutputStream.h"
#include "org/esb/av/PacketInputStream.h"
#include "org/esb/av/PacketOutputStream.h"
#include "org/esb/av/Decoder.h"
#include "org/esb/av/Encoder.h"
#include "org/esb/av/FrameFormat.h"
#include "org/esb/av/FrameConverter.h"
#include "org/esb/hive/job/ProcessUnit.h"

#include <iostream>


using namespace std;
using namespace org::esb;


int main(int argc, char ** argv){
	if(argc!=3){
		cout << argv[0]<<" <infile> <outfile>"<<endl;
		exit(0);
	}

  av_register_all ();

  avcodec_init ();
  avcodec_register_all ();
	
	io::File infile(argv[1]);
	io::File outfile(argv[2]);

	av::FormatInputStream fis(&infile);
	av::FormatOutputStream fos(&outfile);

	av::PacketInputStream pis(&fis);
	av::PacketOutputStream pos(&fos);
	
	
	AVFormatContext *ic = fis.getFormatContext();

	int64_t start=ic->start_time;
//    cout << "Max B Frames :"<<ic->streams[0]->codec->max_b_frames;

  av::FrameFormat format;
  format.pixel_format = PIX_FMT_YUV420P;
  format.height = ic->streams[0]->codec->height;
  format.width = ic->streams[0]->codec->width;


  av::FrameFormat informat;
  format.pixel_format = ic->streams[0]->codec->pix_fmt;
  format.height = ic->streams[0]->codec->height;
  format.width = ic->streams[0]->codec->width;

  av::FrameConverter converter (informat,format);



/*	
	av::Decoder dec(ic->streams[0]->codec->codec_id);
	dec.setStartTime(start);
	dec.setWidth(ic->streams[0]->codec->width);
	dec.setHeight(ic->streams[0]->codec->height);
	dec.open();
*/
//	av::Encoder enc(CODEC_ID_H264);
	av::Encoder enc2(CODEC_ID_MSMPEG4V3);
  	enc2.setWidth (format.width);
  	enc2.setHeight (format.height);
  	enc2.setTimeBase ((AVRational) {1, 25});
  	enc2.setBitRate (4000000);
  	enc2.setGopSize (25);
  	enc2.setPixelFormat (PIX_FMT_YUV420P);
  	enc2.open ();
	
	pos.setEncoder(enc2, 0);

	pos.init();

	
	int b=0;
	int a=0;

for(int b=0;b<10;b++){	
	hive::job::ProcessUnit u;

	av::Decoder dec(ic->streams[0]->codec->codec_id);
	dec.setStartTime(start);
	dec.setWidth(ic->streams[0]->codec->width);
	dec.setHeight(ic->streams[0]->codec->height);
	dec.open();

//	av::Encoder enc(CODEC_ID_H264);
	av::Encoder enc(CODEC_ID_MSMPEG4V3);
  	enc.setWidth (format.width);
  	enc.setHeight (format.height);
  	enc.setTimeBase ((AVRational) {1, 25});
  	enc.setBitRate (4000000);
  	enc.setGopSize (25);
  	enc.setPixelFormat (PIX_FMT_YUV420P);
  	enc.open ();


	u._decoder=&dec;
	u._encoder=&enc;
	for(int a=0;a<200;a++){
		av::Packet p;
		pis.readPacket(p);
		if(p.packet->stream_index!=0 )continue;
//		p.packet->pts-=start;
//		p.packet->dts-=start;
		boost::shared_ptr<Packet> p1(new Packet(p));
		u._input_packets.push_back(p1);
	}
	cout << "StartProcessUnit"<<endl;
	u.process();
	
	list<boost::shared_ptr<Packet> >::iterator bla;
	for(bla=u._output_packets.begin();bla != u._output_packets.end();bla++){
		boost::shared_ptr<Packet> p3=*bla;
//		cout <<"WritePacketPts:"<<p3->packet->pts<<endl;
		p3->packet->pts=++a;//AV_NOPTS_VALUE;
		p3->packet->dts=AV_NOPTS_VALUE;
//		p3->packet->duration=1;
		pos.writePacket(*p3);
//		cout <<"hier"<<endl;
	}
//	u._input_packets.clear();
//	u._output_packets.clear();
}
	
}

