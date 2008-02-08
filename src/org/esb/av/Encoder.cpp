#include "Encoder.h"

#include "Frame.h"
#include "Packet.h"
using namespace org::esb::av;

Encoder::Encoder(CodecID id): Codec(id,Codec::ENCODER){


}

Packet* Encoder::encode(Frame & frame){
    Packet * pac=new Packet();
    int buffer_size=1024*256;
    uint8_t * buffer=new  uint8_t[buffer_size];
    int ret=avcodec_encode_video(this,buffer, buffer_size, &frame);
    pac->data=buffer;
    pac->size=ret;
    pac->pts=frame.pts;
    pac->duration=1;
    pac->flags=0;
    return pac;
}
