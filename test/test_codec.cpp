
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


#include <list>
#include <vector>
#include "boost/shared_ptr.hpp"
using namespace org::esb::av;
using namespace org::esb::config;
using namespace org::esb::io;
using namespace org::esb::config;
using namespace org::esb::hive::job;

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




//    std::vector< std::vector<double> > v=test();
//    cout << v[0][0]<<endl;
//    return 0;

//	Config::init("cluster.cfg");
//	cout << "bla fsel:"<<Config::getProperty("hive.min_frame_group_count")<<endl;
//  log_init ("log.properties");
  av_register_all ();

  avcodec_init ();
  avcodec_register_all ();

/*
  int pts = atoi(argv[1]);
  int test=av_rescale_q(pts,(AVRational){1,16000},(AVRational){32,1225});
//  int test_audio=av_rescale(100,125,AV_TIME_BASE*3);
  cout <<"av_rescale():"<<test<<endl;
  return 0;
*/
  char *filename = argv[1];

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
//      int ret = av_read_frame(ic, pkt);


  File file (filename);
  FormatInputStream fois (&file);
  AVFormatContext *ic = fois.getFormatContext ();

  fois.dumpFormat ();
  PacketInputStream pis (fois);
//    cout << "Width"<<ic->streams[0]->codec->width<<endl;

//    AVPacket pkt;
  int got_picture;

/*
	ProcessUnit unit;	
//	unit._decoder=&dec;
//	unit._encoder=&enc;

	AVCodec * decodec= avcodec_find_decoder(ic->streams[0]->codec->codec_id);
//	cc=avcodec_alloc_context();

	AVCodecContext * decc=avcodec_alloc_context();//ic->streams[0]->codec;
//	AVCodecContext * decc=ic->streams[0]->codec;
	decc->width=enc.getWidth();
	decc->height=enc.getHeight();
	decc->time_base.num=1;
	decc->time_base.den=25;

	if (avcodec_open(decc, decodec) < 0) {
        fprintf(stderr, "could not open codec\n");
        exit(1);
    }

	
	
	AVFrame * picture;
	uint8_t *picture_buf;
	int size;

	picture = avcodec_alloc_frame();
	if (!picture)
    	    return NULL;
	size = avpicture_get_size(enc.getPixelFormat(), enc.getWidth(), enc.getHeight());
	picture_buf = (uint8_t*)av_malloc(size);
	memset(picture_buf,0,size);
	if (!picture_buf) {
    	    av_free(picture);
    	    return NULL;
	}
	avpicture_fill((AVPicture *)picture, picture_buf,
                   enc.getPixelFormat(), enc.getWidth(), enc.getHeight());









	AVCodecContext * cc;
	cc=avcodec_alloc_context();
//	cc->codec_id=CODEC_ID_MPEG1VIDEO;
//	cc->codec_id=CODEC_ID_MPEG2VIDEO;
//	cout << "CodecIC"<<CODEC_ID_MSMPEG4V3<<endl;
	cc->codec_id=CODEC_ID_MSMPEG4V3;
	cc->codec_type=CODEC_TYPE_VIDEO;
//	cc->max_b_frames=1;
	cc->bit_rate=500000;
	cc->width=enc.getWidth();
	cc->height=enc.getHeight();
	cc->time_base.num=1;
	cc->time_base.den=25;
	cc->gop_size=10;
	cc->pix_fmt=PIX_FMT_YUV420P;
*/
/*
	cc->thread_count=1;
	cc->max_qdiff=0;
	cc->sample_aspect_ratio=(AVRational){1,1};
	cc->rc_override_count=0;
	cc->me_threshold=0;
	cc->intra_dc_precision=0;
	cc->strict_std_compliance=0;
	cc->mb_decision=2;
 */
//      cc->debug|=FF_DEBUG_MV;

//      cc->max_b_frames=1;
/*	
	AVCodec * codec;
	codec=avcodec_find_encoder(cc->codec_id);
	avcodec_open(cc,codec);
//	cc->max_b_frames=1;
	

*/
/*
  int video_outbuf_size = 200000;
  uint8_t *video_outbuf = (uint8_t *) av_malloc (video_outbuf_size);
*/
  int insize = 0, outsize = 0;

  FrameFormat format;
  format.pixel_format = PIX_FMT_YUV420P;
  format.height = 1080;		//ic->streams[0]->codec->height;
  format.width = 1920;		//ic->streams[0]->codec->width;



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
  format.pixel_format = (PixelFormat)dec->getPixelFormat();//_pix_fmt;
  format.height = ic->streams[0]->codec->height;
  format.width = ic->streams[0]->codec->width;

  FrameConverter converter (informat,format);

//      cerr << "Decoder"<<endl;
//      Encoder enc(CODEC_ID_MPEG2VIDEO);
//      enc.max_b_frames=3;



//  Encoder * enc=new Encoder(CODEC_ID_MSMPEG4V3);
  Encoder * enc=new Encoder(CODEC_ID_H264);
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


//  FILE *logfile;
//  logfile = fopen ("stats.out", "w");
	int a=0;
  for (int i = 0; i < 1000; i++) {
    Packet p;
//    av_init_packet (p.packet);
    pis.readPacket (p);
    if (p.packet->stream_index != 0)
      continue;

    if (p.isKeyFrame()) {
    	cout << "!!!!!!!!!!!!INIT ENCODER - DECODER"<<endl;
      enc->close ();
	  delete enc;
	dec->close();
//	delete dec;
//  dec=new Decoder (ic->streams[0]->codec->codec_id);
//  dec->setWidth (ic->streams[0]->codec->width);
//  dec->setHeight (ic->streams[0]->codec->height);
//  dec->setTimeBase ((AVRational) {1, 25});
//  dec->setPixelFormat (ic->streams[0]->codec->pix_fmt);
//      dec.setGopSize(0);
	dec->open();

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



    cout << i << "read Packet:"<<p.packet->pts;
    cout.flush ();
//          AVFrame * picture= avcodec_alloc_frame();
    insize += p.packet->size;
//              cout << "InputPacketPts:"<<p.packet->pts<<endl;
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

    cout << "decode Packet";
    cout.flush ();

    Frame frame = dec->decode (p);
    if(frame._buffer==0)continue;
    cout << "convert Packet";
    cout.flush ();

    Frame bla;
    bla=frame;
    Frame f = converter.convert (frame);
    cout << "FrameSize:" << f.getSize () << endl;
    frame.pts = p.packet->pts;
    f.setPts( p.packet->dts);
    f.setDts( p.packet->dts);

    cout << "encode Packet";
    cout.flush ();
    Packet pe = enc->encode (f);
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
		cout << "InputPts:"<<p.packet->pts<<"\tOutputPts:"<<pe.packet->pts<<endl;	
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
