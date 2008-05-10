
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
#include "org/esb/av/FrameConverter.h"
#include "org/esb/av/FrameFormat.h"
#include "org/esb/util/Log.h"
#include "org/esb/config/config.h"
#include "org/esb/hive/CodecFactory.h"

#include <list>
#include <vector>
#include "boost/shared_ptr.hpp"
using namespace org::esb::av;
using namespace org::esb::config;
using namespace org::esb::io;
using namespace org::esb::config;
using namespace org::esb::hive::job;
using namespace org::esb::hive;

logger ("main")



#include "org/esb/av/Decoder.h"
//#include "avformat.h"
//#include "avcodec.h"
#include <iostream>
     using namespace std;

//void testAV();


/* prepare a dummy image */
     static void fill_yuv_image (AVFrame * pict, int frame_index, int width,
				 int height)
{
  int x, y, i;

  i = frame_index;

  /* Y */
  for (y = 0; y < height; y++) {
    for (x = 0; x < width; x++) {
      pict->data[0][y * pict->linesize[0] + x] = 0;	// x + y + i * 3;
    }
  }

  /* Cb and Cr */
  for (y = 0; y < height / 2; y++) {
    for (x = 0; x < width / 2; x++) {
      pict->data[1][y * pict->linesize[1] + x] = 0;	//128 + y + i * 2;
      pict->data[2][y * pict->linesize[2] + x] = 0;	//64 + x + i * 5;
    }
  }
}


int main (int argc, char **argv)
{

	Config::init("cluster.cfg");
	loginit();
	Encoder *temp=CodecFactory::getStreamEncoder(1);
  av_register_all ();

  avcodec_init ();
  avcodec_register_all ();

  char *filename = argv[1];



  File file (filename);
  FormatInputStream fois (&file);
  AVFormatContext *ic = fois.getFormatContext ();

  fois.dumpFormat ();
  PacketInputStream pis (fois);
  int got_picture;

  int insize = 0, outsize = 0;




/*
  Encoder enc2 (CODEC_ID_H264);
  enc2.setWidth (format.width);
  enc2.setHeight (format.height);
  enc2.setTimeBase ((AVRational) {1, 25});
  enc2.setBitRate (4000000);
  enc2.setGopSize (250);
  enc2.setPixelFormat (PIX_FMT_YUV420P);
  enc2.open ();
*/





  Decoder * dec=new Decoder (ic->streams[0]->codec->codec_id);
  dec->setWidth (ic->streams[0]->codec->width);
  dec->setHeight (ic->streams[0]->codec->height);
  dec->setTimeBase ((AVRational) {1, 25});
  dec->setPixelFormat (ic->streams[0]->codec->pix_fmt);

//      dec.setGopSize(0);
  dec->open ();

  FrameFormat informat;
  informat.pixel_format = (PixelFormat)dec->getPixelFormat();//_pix_fmt;
  informat.height = ic->streams[0]->codec->height;
  informat.width = ic->streams[0]->codec->width;

  FrameFormat format;
  format.pixel_format = PIX_FMT_YUV420P;
  format.height = ic->streams[0]->codec->height;
  format.width = ic->streams[0]->codec->width;

  FrameConverter converter (informat,format);

//      cerr << "Decoder"<<endl;
//      Encoder enc(CODEC_ID_MPEG2VIDEO);
//      enc.max_b_frames=3;



  Encoder * enc=new Encoder(CODEC_ID_MSMPEG4V3);

//  Encoder * enc=new Encoder(CODEC_ID_H264);
//  Encoder enc (CODEC_ID_H264);
  enc->setWidth (format.width);
  enc->setHeight (format.height);
  enc->setTimeBase ((AVRational) {1, 25});
  enc->setBitRate (4000000);
  enc->setGopSize (25);
  enc->setPixelFormat (PIX_FMT_YUV420P);
//      enc.setFlag(CODEC_FLAG_PASS1);
  enc->open ();

  File fout (argv[2]);
  FormatOutputStream ffos (&fout);
  PacketOutputStream pos (&ffos);
  pos.setEncoder (*enc, 0);
  pos.init ();
//  		fois.seek(0,44289);//38978


//  FILE *logfile;
//  logfile = fopen ("stats.out", "w");
	int a=0;
	bool firstSeek=false;
  for (int i = 0;i < 1000; i++) {
    Packet p;
//    av_init_packet (p.packet);
    if(pis.readPacket (p)<0)break;
    if (p.packet->stream_index != 0)
      continue;

    if (false&&p.isKeyFrame()) {
    	cout << "!!!!!!!!!!!!INIT ENCODER - DECODER"<<endl;
      enc->close ();
	  delete enc;


//	dec->close();
//	delete dec;
//  dec=new Decoder (ic->streams[0]->codec->codec_id);
//  dec->setWidth (ic->streams[0]->codec->width);
//  dec->setHeight (ic->streams[0]->codec->height);
//  dec->setTimeBase ((AVRational) {1, 25});
//  dec->setPixelFormat (ic->streams[0]->codec->pix_fmt);
//      dec.setGopSize(0);
//	dec->open();

      enc=new Encoder(CODEC_ID_H264);
//      enc=new Encoder(CODEC_ID_MSMPEG4V3);
//  Encoder enc (CODEC_ID_H264);
	  enc->setWidth (format.width);
  	  enc->setHeight (format.height);
  	  enc->setTimeBase ((AVRational) {1, 25});
  	  enc->setBitRate (4000000);
  	  enc->setGopSize (25);
  	  enc->setPixelFormat (PIX_FMT_YUV420P);
//      enc.setFlag(CODEC_FLAG_PASS1);
  	  enc->open ();
//      enc->open ();
    }






//          enc.thread_count=1;
//          enc.max_qdiff=0;
//          enc.sample_aspect_ratio=(AVRational){1,1};
//          enc.rc_override_count=0;
//          enc.me_threshold=0;
//          enc.intra_dc_precision=0;
//          enc.strict_std_compliance=0;
//          enc.debug|=FF_DEBUG_MV;

//      enc.open();



//    cout << i << "read Packet:"<<p.packet->pts;
//    cout.flush ();
//          AVFrame * picture= avcodec_alloc_frame();
    insize += p.packet->size;
    cout << "InputPacketPts:"<<p.packet->pts;
    cout << "\tInputPacketDts:"<<p.packet->dts;
    cout << endl;
//              boost::shared_ptr<Packet> ptr(new Packet(p));
//      cout << "InputPacketSize:"<<ptr->packet->size<<endl;
//              Frame frame(enc.getPixelFormat(), enc.getWidth(), enc.getHeight());
//          unit._input_packets.push_back(ptr);
//          dec.debug|=FF_DEBUG_MV;
//          fill_yuv_image(picture, i, enc.getWidth(), enc.getHeight());

    int out_size = 0;
//          out_size = avcodec_decode_video(decc, &frame, &got_picture, p.packet->data, p.packet->size);
//          picture->pict_type=1;
//          picture->quality=dec.coded_frame->quality;

//    cout << "decode Packet";
//    cout.flush ();

    Frame tmp=dec->decode(p);
    if(tmp._buffer==0)continue;
    Frame f = converter.convert (tmp);
    cout << "FrameSize:" << f.getSize () << endl;
//    frame.pts = p.packet->pts;
    f.setPts( p.packet->dts);
//    f.setDts( p.packet->dts);

    cout << "encode Packet";
    cout.flush ();
    Packet pe = enc->encode (f);
		if(!firstSeek){
  			fois.seek(0,44289);
  			firstSeek=true;
  		}

//    if (enc.ctx->stats_out)
//      fprintf (logfile, "%s", enc.ctx->stats_out);

//          pe.packet->pts=p.packet->pts;
//          pe.packet->dts=p.packet->dts;
//          pe.packet->stream_index=0;

    cout << "write Packet";
    cout.flush ();
    pe.packet->pts=++a;
    pe.packet->dts=AV_NOPTS_VALUE;;
    
    pos.writePacket (pe);
    out_size = pe.packet->size;

//          fill_yuv_image(&f, i, enc.getWidth(), enc.getHeight());



//          picture->pts = AV_NOPTS_VALUE;//p.packet->pts;
//          frame.pts = p.packet->pts;//AV_NOPTS_VALUE;//p.packet->pts;

//          out_size = avcodec_encode_video(enc.ctx, video_outbuf, video_outbuf_size, &frame);
    outsize += out_size;
//		cout << "InputPts:"<<p.packet->pts<<"\tOutputPts:"<<pe.packet->pts<<endl;	
//    cout << "InputPacketSize:" << p.packet->size << "\tOutputPacketSize:" << out_size << "\tKeyFrame:" << enc.ctx->coded_frame->key_frame << endl;

//          cout <<"FrameHere:"<<out_size<<endl;
//          outsize+=out_size;
//          Packet pe=enc.encode(f);
//          cout << "EncPacketSize:"<<pe.packet->size<<endl;

//              av_free(picture);
  }
  cout << "InputPacketSizeAll:" << insize << "\tOutputPacketSizeAll:" <<
    outsize << endl;
//  pos.close();
  
  
//  Config::close();
/*
    av_free(video_outbuf);
	avcodec_close(cc);
	avcodec_close(decc);
	av_free(cc);
	av_free(decc);
//	av_free(decodec);
//	av_free(codec);
	av_free(picture);
	av_free(picture_buf);
*/
/*	
	FileOutputStream fos("test.unit");
	ObjectOutputStream oos(&fos);
	oos.writeObject(unit);
	fos.flush();
	cout << "Data Serailized"<<endl;
*/
  /*
     ProcessUnit unit2;   
     FileInputStream fis("test.unit");
     ObjectInputStream ois(&fis);
     ois.readObject(unit2);

     unit2.process();
   */
//      delete unit2._decoder;
//      delete unit2._encoder;


/*
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


//              Frame f=dec.decode(p);
//              Packet p2=enc.encode(f);
//              boost::shared_ptr<Packet> ptr(new Packet(p));
//              unit._input_packets.push_back(ptr);

//      unit.process();
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
//      delete unit2._decoder;


//      Config::close();
}
