#include "Encoder.h"

#include "Frame.h"
#include "Packet.h"
using namespace org::esb::av;

Encoder::Encoder(CodecID id): Codec(id,Codec::ENCODER){


}

Encoder::Encoder(): Codec(){


}

Packet Encoder::encode(Frame & frame){
	return encodeVideo(frame);
}
Packet Encoder::encodeVideo(Frame & frame){
    int buffer_size=1024*256;
    uint8_t data[buffer_size];
    memset(data,0,buffer_size);
    int ret=avcodec_encode_video(this,data, buffer_size, &frame);
//    pac.data=new uint8_t[ret];

    Packet pac(ret);
    memcpy(pac.data, data, ret);
//    pac.data=data;
    pac.size=ret;
    pac.pts=frame.pts;
    pac.dts=frame.dts;
    pac.duration=1;
//    pac.flags=0;

    if(coded_frame && coded_frame->key_frame)
	pac.flags |= PKT_FLAG_KEY;
    return pac;
}

Packet Encoder::encodeAudio(Frame & frame){

}
