#include "Encoder.h"

#include "Frame.h"
#include "Packet.h"
#include "org/esb/lang/Exception.h"
#include <iostream>
using namespace org::esb::av;
using namespace org::esb;
using namespace std;

Encoder::Encoder(CodecID id) : Codec(id, Codec::ENCODER) {


}

Encoder::Encoder() : Codec() {


}

Packet Encoder::encode(Frame & frame) {
    if (ctx->codec_type == CODEC_TYPE_VIDEO)
        return encodeVideo(frame);
    if (ctx->codec_type == CODEC_TYPE_AUDIO)
        return encodeAudio(frame);
    return Packet();
}

Packet Encoder::encodeVideo(Frame & frame) {
//  logdebug("VideoEncoderFrame");
//  frame.toString();
    const int buffer_size = 1024 * 256;
    char data[buffer_size];
    memset(&data, 0, buffer_size);

    int ret = avcodec_encode_video(ctx, (uint8_t*) & data, buffer_size, &frame);
    //    pac.data=new uint8_t[ret];
    //	cout << "ret:"<<ret<<endl;
//    logdebug("\rCreate Packet with Size:"<<ret);
    Packet pac(ret);
    if(ret>0){
      memcpy(pac.packet->data, &data, ret);
//      delete []data;
    }else{
      throw lang::Exception();
    }
    //    pac.data=data;
    pac.packet->size = ret;
    pac.packet->pts = frame.getPts();
    pac.packet->dts = frame.getDts();
    pac.packet->pos = frame.pos;
    pac.packet->stream_index = frame.stream_index;
    pac.packet->duration = frame.duration;
    //    pac.flags=0;

    if (ctx->coded_frame) {
        //		cout <<"!!!!!!!HAVE CODED FRAME!!!!!!!!!!"<<endl;
        if (ctx->coded_frame->key_frame)
            pac.packet->flags |= PKT_FLAG_KEY;
        //		pac.packet->pts= av_rescale_q(ctx->coded_frame->pts, ctx->time_base, (AVRational){1,25});
        //		pac.packet->pts=ctx->coded_frame->pts;
    }
    return pac;
}

Packet Encoder::encodeAudio(Frame & frame) {
//  logdebug("AudioEncoderFrame");
//  frame.toString();
    const int outbuf_size = 10000;
    char outbuf[outbuf_size];
    int out_size = avcodec_encode_audio(ctx, (uint8_t*) & outbuf, outbuf_size, (short int *) frame._buffer);
    Packet pak(out_size);
    pak.packet->size = out_size;
    memcpy(pak.packet->data, &outbuf, out_size);
    pak.packet->pts = frame.getPts();
    //	pak.pts=this->coded_frame->pts;
    //    if(coded_frame && coded_frame->pts != AV_NOPTS_VALUE)
    //    	pak.pts= av_rescale_q(coded_frame->pts, time_base, (AVRational){1,15963});


    if (ctx->coded_frame) {
        //		pak.packet->pts=ctx->coded_frame->pts;
        //		cout <<"Encoder Audio Pts:"<<ctx->coded_frame->pts<<endl;
        //	    pak.packet->duration=ctx->coded_frame->duration;
    }
    pak.packet->flags |= PKT_FLAG_KEY;
    pak.packet->stream_index = frame.stream_index;

    pak.packet->dts = frame.getDts();
    //	pak.packet->pos=frame.pos;
    pak.packet->duration = frame.duration;
    //	cout << "FramePts:"<<frame.pts<<"\tEncodedPts"<<pak.pts<<endl;	
    return pak;
}
