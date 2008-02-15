#include "Decoder.h"
#include "Frame.h"

using namespace org::esb::av;


Decoder::Decoder(CodecID id): Codec(id,Codec::DECODER){}

Frame Decoder::decode(Packet & packet){
    Frame frame(pix_fmt, width, height);
    int _frameFinished=0;
    frame._width=this->width;
    frame._height=this->height;
    frame._pixFormat=this->pix_fmt;
    int bytesRemaining=packet.size,  bytesDecoded=0;
    uint8_t * rawData=packet.data;
    while(bytesRemaining > 0){
	bytesDecoded=avcodec_decode_video(this, &frame, &_frameFinished, rawData, bytesRemaining);
	if(bytesDecoded < 0){
	    fprintf(stderr, "Error while decoding frame\n");
	    break;
	}
	bytesRemaining-=bytesDecoded;
	rawData+=bytesDecoded;
	if(_frameFinished){
	    break;
	}
    }
    frame._pixFormat=pix_fmt;
    frame.pts=packet.pts;
    frame.dts=packet.dts;
    return frame;
}
