#include "Decoder.h"
#include "Frame.h"

#include <iostream>
using namespace org::esb::av;
using namespace std;

Decoder::Decoder(): Codec(){}
Decoder::Decoder(CodecID id): Codec(id,Codec::DECODER){}

Frame Decoder::decode(Packet & packet){
	if(this->codec_type==CODEC_TYPE_VIDEO)
		return decodeVideo(packet);
	if(this->codec_type==CODEC_TYPE_AUDIO)
		return decodeAudio(packet);
}
/*
void Decoder::decodeVideo(Packet & packet, Frame & frame){
//    Frame frame(pix_fmt, width, height);
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
*/
Frame Decoder::decodeVideo(Packet & packet){
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
    frame.pos=packet.pos;
    return frame;
}

Frame Decoder::decodeAudio(Packet & packet){
//	cout << "DecodeAudio Packet"<<endl;
//        Frame frame;
	int size=packet.size, out_size;
	uint8_t * outbuf=new uint8_t[AVCODEC_MAX_AUDIO_FRAME_SIZE];
	uint8_t * inbuf=packet.data;
        while (size > 0) {
            int len = avcodec_decode_audio(this, (short *)outbuf, &out_size, inbuf, size);
            if (len < 0) {
                fprintf(stderr, "Error while decoding\n");
                exit(1);
            }
            if (out_size > 0) {
                /* if a frame has been decoded, output it */
//                fwrite(outbuf, 1, out_size, outfile);
            }
            size -= len;
            inbuf += len;
        }
//		cout << "DataSize:"<<out_size<<endl;
// 		cout <<"PacketPts:"<<packet.pts<< "\tDecodedFramePts:"<<this->coded_frame->pts<<endl;
        Frame frame;
        frame._buffer=outbuf;
        frame.pts=packet.pts;
        frame.dts=packet.dts;
        frame.pos=packet.pos;
        frame.duration=packet.duration;
        frame._size=out_size;
    return frame;
}
