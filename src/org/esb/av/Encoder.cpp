#include "Encoder.h"

#include "Frame.h"
#include "Packet.h"
#include <iostream>
using namespace org::esb::av;
using namespace std;
Encoder::Encoder(CodecID id): Codec(id,Codec::ENCODER){


}

Encoder::Encoder(): Codec(){


}

void Encoder::encode(Frame & frame, Packet & p){
    int buffer_size=1024*256;
    uint8_t data[buffer_size];
    memset(&data,0,buffer_size);
    
    int ret=avcodec_encode_video(this,(uint8_t*)&data, buffer_size, &frame);
//    pac.data=new uint8_t[ret];
/*
    Packet pac(ret);
    memcpy(pac.packet->data, &data, ret);
//    pac.data=data;
    pac.packet->size=ret;
    pac.packet->pts=frame.pts;
    pac.packet->dts=frame.dts;
    pac.packet->pos=frame.pos;
    pac.packet->duration=1;
//    pac.flags=0;

    if(coded_frame && coded_frame->key_frame)
	pac.packet->flags |= PKT_FLAG_KEY;
	*/
//    return pac;
}


Packet Encoder::encode(Frame & frame){
	if(this->codec_type==CODEC_TYPE_VIDEO)
	    return encodeVideo(frame);
	if(this->codec_type==CODEC_TYPE_AUDIO)
	    return encodeAudio(frame);
	return Packet();
}
Packet Encoder::encodeVideo(Frame & frame){



    int buffer_size=1024*256;
    uint8_t data[buffer_size];
    memset(&data,0,buffer_size);
    
    int ret=avcodec_encode_video(this,(uint8_t*)&data, buffer_size, &frame);
//    pac.data=new uint8_t[ret];

    Packet pac(ret);
    memcpy(pac.packet->data, &data, ret);
//    pac.data=data;
    pac.packet->size=ret;
    pac.packet->pts=frame.pts;
    pac.packet->dts=frame.dts;
    pac.packet->pos=frame.pos;
    pac.packet->duration=1;
//    pac.flags=0;

    if(coded_frame && coded_frame->key_frame)
	pac.packet->flags |= PKT_FLAG_KEY;
	
    return pac;
}

Packet Encoder::encodeAudio(Frame & frame){
	int outbuf_size=10000;
	uint8_t outbuf[outbuf_size];
        int out_size = avcodec_encode_audio(this, (uint8_t*)&outbuf, outbuf_size, (short int *)frame._buffer);
	Packet pak(out_size);
	pak.packet->size=out_size;
	memcpy(pak.packet->data,&outbuf,out_size);
	pak.packet->pts=frame.pts;
//	pak.pts=this->coded_frame->pts;
//    if(coded_frame && coded_frame->pts != AV_NOPTS_VALUE)
//    	pak.pts= av_rescale_q(coded_frame->pts, time_base, (AVRational){1,15963});

	pak.packet->dts=frame.dts;
	pak.packet->pos=frame.pos;
	pak.packet->duration=frame.duration;
//	cout << "FramePts:"<<frame.pts<<"\tEncodedPts"<<pak.pts<<endl;	
    return pak;
}
