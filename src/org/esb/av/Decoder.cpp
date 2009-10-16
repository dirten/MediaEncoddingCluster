#include "Decoder.h"
#include "Frame.h"
//#include "VideoFrame.h"
//#include "AudioFrame.h"
#include <iostream>
#include <stdexcept>
#include "org/esb/util/Log.h"
using namespace org::esb::av;
using namespace std;

Decoder::Decoder() : Codec() {
}

Decoder::Decoder(CodecID id) : Codec(id, Codec::DECODER) {
}
Decoder::Decoder(AVCodecContext * c) : Codec(c, Codec::DECODER) {
}

Frame Decoder::decodeLast() {
  Frame frame(ctx->pix_fmt, ctx->width, ctx->height);
  int _frameFinished = 0;
  int bytesDecoded = avcodec_decode_video2(ctx, frame.getAVFrame(), &_frameFinished, NULL);
  if (bytesDecoded < 0) {
    fprintf(stderr, "Error while decoding frame\n");
  }
#if 0
  if (_frameFinished) {
    logdebug("Frame finished");
  } else {
    logdebug("Frame not finished !!!!!");
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

  frame._pixFormat = ctx->pix_fmt;
  //  frame.stream_index=packet.packet->stream_index;
  //  frame.setPts(packet.packet->pts);
  //  frame.setDts( packet.packet->dts );
  //  frame.pos = packet.packet->pos;
  //  frame.duration = packet.packet->duration;
  frame._type = CODEC_TYPE_VIDEO;
  return frame;

}

Frame * Decoder::decode2(Packet & packet) {
  if (!_opened)
    throw runtime_error("Codec not opened");
  if (ctx->codec_type == CODEC_TYPE_VIDEO)
    return decodeVideo2(packet);
  if (ctx->codec_type == CODEC_TYPE_AUDIO)
    return decodeAudio2(packet);
  
  throw runtime_error("Packet is no type of Video or Audio");
  //  return Frame ();
}
/*
void Decoder::analyzePacket(Packet & packet) {
  int _frameFinished = 0;
  Frame frame(ctx->pix_fmt, ctx->width, ctx->height);

  int bytesDecoded = avcodec_decode_video(ctx, frame.getAVFrame(), &_frameFinished, packet.packet->data, packet.packet->size);
  if (bytesDecoded < 0) {
    fprintf(stderr, "Error while decoding frame\n");
  }
}
*/

Frame * Decoder::decodeVideo2(Packet & packet) {
  Frame * frame=new Frame(ctx->pix_fmt, ctx->width, ctx->height, false);
  int _frameFinished = 0;
  int len = packet.packet->size;
#ifdef DEBUG
  logdebug("InputPacketData:");
  packet.toString();
#endif

  //  while (len > 0) {
  //    logdebug("Decode Packet");
  int bytesDecoded =
      avcodec_decode_video2(ctx, frame->getAVFrame(), &_frameFinished, packet.packet);
#ifdef DEBUG
  logdebug("BytesDecoded:"<<bytesDecoded);
#endif
  if (bytesDecoded < 0) {
    fprintf(stderr, "Error while decoding frame\n");
  }
  if (_frameFinished) {
    //      logdebug("Frame Finished");
    //      break;
  }
  len -= bytesDecoded;
  //  }
#if 0

  if (_frameFinished) {
    logdebug("Frame finished");
  } else {

    logdebug("Frame not finished !!!!!");
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
    //        cout << endl;

  }

  logdebug("\tPacketFrameType:");
  switch (frame->getAVFrame()->pict_type) {
    case FF_B_TYPE:
      logdebug("B");
      break;
    case FF_I_TYPE:
      logdebug("I");
      break;
    case FF_P_TYPE:
      logdebug("P");
      break;
    case FF_S_TYPE:
      logdebug("S");
      break;
    case FF_SI_TYPE:
      logdebug("SI");
      break;
    case FF_SP_TYPE:
      logdebug("SP");
      break;
    case FF_BI_TYPE:
      logdebug("BI");
      break;
    default:
      logdebug("U:" << frame->getAVFrame()->pict_type);
      break;

  }

  //    cout << endl;
#endif
  if (!_frameFinished) {
    return frame;
  }
  frame->setTimeBase(packet.getTimeBase());
  frame->setFinished(_frameFinished);
  frame->_pixFormat = ctx->pix_fmt;
  frame->stream_index = packet.packet->stream_index;
  frame->setPts(packet.packet->pts);
  frame->setDts(packet.packet->dts);
  frame->pos = packet.packet->pos;
  frame->duration = packet.packet->duration;
  frame->_type = CODEC_TYPE_VIDEO;
#ifdef DEBUG
  logdebug("Decoded Video Frame");
  frame->toString();

#endif
  return frame;
}

Frame * Decoder::decodeAudio2(Packet & packet) {
// logdebug("DecodeAudio Packet:"<<packet.toString());
  //        Frame frame;
  int size = packet.packet->size, samples_size = AVCODEC_MAX_AUDIO_FRAME_SIZE;
//  uint8_t *outbuf = new uint8_t[samples_size];
  uint8_t *outbuf = (uint8_t*)av_malloc(samples_size);
  //    uint8_t *inbuf = packet.packet->data;
  //    while (size > 0) {
//  int len = avcodec_decode_audio2(ctx, (short *) outbuf, &samples_size, packet.packet->data, size);
  int len = avcodec_decode_audio3(ctx, (short *) outbuf, &samples_size, packet.packet);
//  logdebug("DecodingLength:"<<len<<" PacketSize:"<<packet.getSize()<<"SampleSize:"<<samples_size<<"FrameSize:"<<ctx->frame_size*ctx->channels);
  if (len < 0) {
    logerror("Error while decoding audio Frame");
    return new Frame();
  }
  if (samples_size > 0) {
    /* if a frame has been decoded, output it */
    //                fwrite(outbuf, 1, out_size, outfile);
  }
  size -= len;
  //        inbuf += len;
  //    }
  //              cout << "DataSize:"<<out_size<<endl;
  //              cout <<"PacketPts:"<<packet.pts<< "\tDecodedFramePts:"<<this->coded_frame->pts<<endl;
  Frame * frame=new Frame(outbuf);
  frame->_allocated=true;
//  frame._buffer = outbuf;
  frame->setTimeBase(packet.getTimeBase());
  frame->stream_index = packet.packet->stream_index;
  frame->setPts(packet.packet->pts);
  frame->setDts(packet.packet->dts);
  frame->pos = packet.packet->pos;
  frame->duration = packet.packet->duration;
  frame->_size = samples_size;
  frame->_type = CODEC_TYPE_AUDIO;
  frame->channels = ctx->channels;
  frame->sample_rate = ctx->sample_rate;
  frame->setFinished(true);
//  logdebug("DecodedAudioFrame:"<<frame->toString());
  return frame;
}
