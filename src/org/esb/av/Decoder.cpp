#include "Decoder.h"
#include "Frame.h"
//#include "VideoFrame.h"
//#include "AudioFrame.h"

#include <iostream>
using namespace org::esb::av;
using namespace std;

Decoder::Decoder ():Codec ()
{
}

Decoder::Decoder (CodecID id):Codec (id, Codec::DECODER)
{
}

Frame Decoder::decode (Packet & packet)
{
  if(!_opened)
    throw runtime_error("Codec not opened");
  if (ctx->codec_type == CODEC_TYPE_VIDEO)
    return decodeVideo (packet);
  if (ctx->codec_type == CODEC_TYPE_AUDIO)
    return decodeAudio (packet);
  throw runtime_error("Packet is no type of Video or Audio");
//  return Frame ();
}

Frame Decoder::decodeVideo (Packet & packet)
{
  Frame frame (_pix_fmt, _width, _height);
  int _frameFinished = 0;
int len=packet.packet->size;
while(len>0){
  int bytesDecoded =
    avcodec_decode_video (ctx, &frame, &_frameFinished, packet.packet->data,
			  packet.packet->size);
  if (bytesDecoded < 0) {
    fprintf (stderr, "Error while decoding frame\n");
  }
    if(_frameFinished){
    	break;
    }
  len-=bytesDecoded;
}
#if 1
  if(_frameFinished){
    cout <<"Frame finished"<<endl;
  }else{
  	
    cout <<"Frame not finished !!!!!"<<endl;  
/*
     int bla= avcodec_decode_video(ctx, &frame, &_frameFinished, NULL, 0);
  if (bla < 0) {
    fprintf (stderr, "Error while decoding frame\n");
  }
  if(_frameFinished){
    cout <<"Frame finished in second try"<<endl;
  }else{
    cout <<"Frame not finished in second try!!!!!"<<endl;  
     return Frame();
  }
  */
//      return Frame();
  }
#endif

  frame._pixFormat = _pix_fmt;
  frame.stream_index=packet.packet->stream_index;
  frame.setPts(packet.packet->pts);
  frame.setDts( packet.packet->dts );
  frame.pos = packet.packet->pos;
  frame.duration = packet.packet->duration;
  frame._type=CODEC_TYPE_VIDEO;
  return frame;
}

Frame Decoder::decodeAudio (Packet & packet)
{
//      cout << "DecodeAudio Packet"<<endl;
//        Frame frame;
  int size = packet.packet->size, out_size=AVCODEC_MAX_AUDIO_FRAME_SIZE;
  uint8_t *outbuf = new uint8_t[AVCODEC_MAX_AUDIO_FRAME_SIZE];
  uint8_t *inbuf = packet.packet->data;
  while (size > 0) {
    int len =avcodec_decode_audio2 (ctx, (short *) outbuf, &out_size, inbuf, size);
    if (len < 0) {
      fprintf (stderr, "Error while decoding audio Frame\n");
//                exit(1);
    }
    if (out_size > 0) {
      /* if a frame has been decoded, output it */
//                fwrite(outbuf, 1, out_size, outfile);
    }
    size -= len;
    inbuf += len;
  }
//              cout << "DataSize:"<<out_size<<endl;
//              cout <<"PacketPts:"<<packet.pts<< "\tDecodedFramePts:"<<this->coded_frame->pts<<endl;
  Frame frame;
  frame._buffer = outbuf;
  frame.stream_index=packet.packet->stream_index;
  frame.setPts( packet.packet->pts);
  frame.setDts( packet.packet->dts);
  frame.pos = packet.packet->pos;
  frame.duration = packet.packet->duration;
  frame._size = out_size;
  frame._type=CODEC_TYPE_AUDIO;
  frame.channels=ctx->channels;
  frame.sample_rate=ctx->sample_rate;
  
  return frame;
}
