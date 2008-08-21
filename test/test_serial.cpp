#include "org/esb/av/FormatInputStream.h"
#include "org/esb/av/PacketInputStream.h"
#include "org/esb/av/Packet.h"
#include "org/esb/av/Frame.h"
#include "org/esb/av/FrameFormat.h"
#include "org/esb/av/FrameConverter.h"
#include "org/esb/av/FormatOutputStream.h"
#include "org/esb/io/File.h"
#include "org/esb/io/FileOutputStream.h"
#include "org/esb/io/FileInputStream.h"
#include "org/esb/io/ObjectOutputStream.h"
#include "org/esb/io/ObjectInputStream.h"

#include <boost/shared_ptr.hpp>
#include "org/esb/hive/job/ProcessUnit.h"

using namespace org::esb::io;
using namespace org::esb::av;
using namespace org::esb::hive::job;



int main(int argc, char ** argv){
	loginit();
  av_register_all ();

  avcodec_init ();
  avcodec_register_all ();

	File file(argv[1]);
	FormatInputStream fis(&file);
	PacketInputStream pis(&fis);

    
//    Codec c;  
    FrameFormat format;
    format.pixel_format = PIX_FMT_YUV420P;
    format.height = 1080;		//ic->streams[0]->codec->height;
    format.width = 1920;		//ic->streams[0]->codec->width;
    FrameConverter converter (format);

    int codec_id=13;//fis.getAVStream(0)->getCodec()->ctx->codec_id;
    int width=512;//fis.getAVStream(0)->getCodec()->ctx->width;
    int height=256;//fis.getAVStream(0)->getCodec()->ctx->height;
    int pix_fmt=0;//fis.getAVStream(0)->getCodec()->ctx->pix_fmt;


    Decoder dec((CodecID)codec_id);
    dec.setWidth (width);
    dec.setHeight (height);
    dec.setTimeBase ((AVRational) {1, 25});
    dec.setPixelFormat ((PixelFormat)pix_fmt);
    dec.open ();

//    Encoder enc(CODEC_ID_MSMPEG4V3);
//  Encoder enc (CODEC_ID_H263);
  Encoder enc (CODEC_ID_H264);
//  Encoder enc (CODEC_ID_MPEG2VIDEO);
    enc.setWidth (format.width);
    enc.setHeight (format.height);
    enc.setTimeBase ((AVRational) {1, 25});
    enc.setBitRate (4000000);
    enc.setGopSize (250);
    enc.setPixelFormat (PIX_FMT_YUV420P);
//      enc.setFlag(CODEC_FLAG_PASS1);
    enc.open ();

	
	ProcessUnit unit;
/*
    Codec * c=new Codec((CodecID)codec_id);
    c->setWidth (width);
    c->setHeight (height);
    c->setTimeBase ((AVRational) {1, 25});
    c->setPixelFormat ((PixelFormat)pix_fmt);
  */
  //  c->open();
//	unit.codec=c;
	unit._decoder=&dec;
	unit._encoder=&enc;
	for(int a=0;a<250;a++){
      std::cout << "Loop:"<<a<<std::endl;
		Packet p;
		pis.readPacket(p);
		if(p.packet->stream_index!=0)continue;
		boost::shared_ptr<Packet>packet(new Packet(p));
		unit._input_packets.push_back(packet);
//        Frame f=dec.decode(*packet);
//        Frame f2=converter.convert(f);
//        f.pts = p.packet->pts;
//        f.dts = p.packet->dts;
//        Packet p2=enc.encode(f2);
	}
	/*
		int insize=0, outsize=0;

	list< boost::shared_ptr<Packet> >::iterator it; 
	for(it=unit._input_packets.begin();it!=unit._input_packets.end();it++){
		
	    boost::shared_ptr<Packet> p=*it;
	    insize+=p->packet->size;
	    cout << "start decoding insize:"<<p->packet->size<<endl;

	    Frame tmp=unit._decoder->decode(*p);

		Frame f=converter.convert(tmp);
	    cout << "FrameSize:"<<f.getSize()<<endl;
	    Packet ret=unit._encoder->encode(f);
	    
	}
*/

  //  unit.process();
	
	FileOutputStream fos("new.unit");
	ObjectOutputStream oos(&fos);
	oos.writeObject(unit);
	oos.close();
//    dec.close();	
	cout << "Data Serailized"<<endl;
//	delete c;
//	delete dec;
//	delete unit._decoder;
//	delete unit._encoder;
/*
     ProcessUnit unit2;   
     FileInputStream ffis("new.unit");
     ObjectInputStream ois(&ffis);
     ois.readObject(unit2);

   //  unit2.process();
  delete unit2._decoder;
  delete unit2._encoder;
  */
//  av_free_static();
}