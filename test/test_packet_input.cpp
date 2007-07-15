#include "org/esb/io/FileInputStream.h"
#include "org/esb/io/FileOutputStream.h"
#include "org/esb/av/Frame.h"
#include "org/esb/av/CodecInputStream.h"
#include "org/esb/av/PacketInputStream.h"
#include "org/esb/av/PacketOutputStream.h"
#include "avformat.h"
#include <iostream>

using namespace org::esb::av;
using namespace org::esb::io;



int main(){
     av_register_all();

	int bundleCounter=1;
	char filename[100];







	sprintf(filename,"/tmp/hive/con%04d.data", bundleCounter);
	FileInputStream fis(filename);
	
	cout << "available:"<<fis.available()<<endl;
	CodecInputStream cis(&fis);
	PacketInputStream pis(&fis);	
	Codec * codec=NULL;	
	codec=cis.readCodec();
	codec=cis.readCodec();
	codec->open();
	cout << "available:"<<fis.available()<<endl;
	int packets=0;
	AVCodecContext *cc=avcodec_alloc_context();
    cc->codec_type = CODEC_TYPE_VIDEO;
	cc->height=codec->height;
	cc->width=codec->width;
	cc->bit_rate=20000000;
	cc->max_b_frames=1;
    cc->time_base.den = 25;
    cc->time_base.num = 1;
    cc->gop_size = 10; /* emit one intra frame every twelve frames at most */
    cc->pix_fmt = PIX_FMT_YUV420P;
//    cc->flags |= CODEC_FLAG_GLOBAL_HEADER;

	char outfile[100];
	sprintf(outfile,"/tmp/hive/bon%04d.out", bundleCounter);
	FileOutputStream fos(outfile);
	PacketOutputStream pos(&fos);

	AVCodec * c=avcodec_find_encoder(CODEC_ID_MPEG1VIDEO);
    if (!c) {
        fprintf(stderr, "codec not found\n");
        exit(1);
    }
	cc->codec_id=c->id;
	
    if (avcodec_open(cc, c) < 0) {
        fprintf(stderr, "could not open codec\n");
        exit(1);
    }

	while(fis.available()>0){
		Packet p=pis.readPacket();


		Frame frame(&p, codec);
		uint8_t * video_outbuf=NULL;
		int video_outbuf_size;
	    video_outbuf_size = 400000;
	    video_outbuf = (uint8_t*)av_malloc(video_outbuf_size);
	    int out_size = avcodec_encode_video(cc, video_outbuf, video_outbuf_size, &frame);





		if (out_size > 0) {
             Packet pkt;
             av_init_packet(&pkt);
 
             pkt.pts= av_rescale_q(cc->coded_frame->pts, cc->time_base, cc->time_base);
             if(cc->coded_frame->key_frame)
                 pkt.flags |= PKT_FLAG_KEY;
             pkt.stream_index= 0;
             pkt.data= video_outbuf;
             pkt.size= out_size;
	      pos.writePacket(&pkt);
             /* write the compressed frame in the media file */
//             ret = av_write_frame(oc, &pkt);
         }




//		fos.write((char*)video_outbuf,out_size);
		delete video_outbuf;
		cout <<++packets<<":Packets read \toutsize:"<<out_size<<endl;
		
	}
	
	
	
	
	
	
}