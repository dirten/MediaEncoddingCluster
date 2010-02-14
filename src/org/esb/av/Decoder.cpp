/*----------------------------------------------------------------------
*  File    : Decoder.cpp
*  Author  : Jan Hölscher <jan.hoelscher@esblab.com>
*  Purpose :
*  Created : 6. November 2009, 12:30 by Jan Hölscher <jan.hoelscher@esblab.com>
*
*
* MediaEncodingCluster, Copyright (C) 2001-2009   Jan Hölscher
*
* This program is free software; you can redistribute it and/or
*  modify it under the terms of the GNU General Public License as
*  published by the Free Software Foundation; either version 2 of the
*  License, or (at your option) any later version.
*
*  This program is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
*  General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with this program; if not, write to the Free Software
*  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
*  02111-1307 USA
*
* ----------------------------------------------------------------------
*/
#include "Decoder.h"
#include "Frame.h"
#include <iostream>
#include <stdexcept>
#include "org/esb/util/Log.h"



using namespace org::esb::av;
using namespace std;

Decoder::Decoder() : Codec(Codec::DECODER) {
  _last_pts = 0;
  _next_pts = 0;
}

Decoder::Decoder(CodecID id) : Codec(id, Codec::DECODER) {
  _last_pts = 0;
  _next_pts = 0;
}

Decoder::Decoder(AVStream * c) : Codec(c, Codec::DECODER) {
  _last_pts = 0;
  _next_pts = 0;
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
    LOGDEBUG("Frame finished");
  } else {
    LOGDEBUG("Frame not finished !!!!!");
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
  LOGTRACEMETHOD("Decode Video");
  Frame * frame = new Frame(ctx->pix_fmt, ctx->width, ctx->height, false);
  int _frameFinished = 0;
  int len = packet.packet->size;
  LOGDEBUG(packet.toString());

  //  while (len > 0) {
  //    logdebug("Decode Packet");
  int bytesDecoded =
    avcodec_decode_video2(ctx, frame->getAVFrame(), &_frameFinished, packet.packet);
  //@TODO: this is a hack, because the decoder changes the TimeBase after the first packet was decoded
  if (false&&_last_pts == AV_NOPTS_VALUE) {
#ifdef USE_TIME_BASE_Q
    _last_pts = av_rescale_q(packet.getPts(), packet.getTimeBase(), AV_TIME_BASE_Q);
#else
    _last_pts = av_rescale_q(packet.getPts(), packet.getTimeBase(), ctx->time_base);
#endif
    LOGDEBUG("setting last pts to " << _last_pts << " ctxtb=" << ctx->time_base.num << "/" << ctx->time_base.den
      << " ptb=" << packet.getTimeBase().num << "/" << packet.getTimeBase().den);
  }

  LOGDEBUG("BytesDecoded:" << bytesDecoded);
  if (bytesDecoded < 0) {
    LOGERROR("Error while decoding frame");
  }
  /**
  * if frame is not finished, returns the blank frame
  * the calling process of decode must ensure to check if the returning frame isFinished by calling the Method isFinished()
  */
  if (!_frameFinished) {
    return frame;
  }

  len -= bytesDecoded;


  //  }
  /* calculating the Presentation TimeStamp here*/
  frame->setPts(_last_pts);
  frame->setDts(_last_pts);

#ifdef USE_TIME_BASE_Q
  frame->setTimeBase(AV_TIME_BASE_Q);
  // calculating the duration of the decoded packet
  int64_t dur = av_rescale_q(packet.packet->duration, packet.getTimeBase(), AV_TIME_BASE_Q);
#else
  frame->setTimeBase(ctx->time_base);
  // calculating the duration of the decoded packet
//    int64_t dur = av_rescale_q(packet.packet->duration, packet.getTimeBase(), ctx->time_base);
//  int64_t tmp_dur=((int64_t)AV_TIME_BASE * ctx->time_base.num * ctx->ticks_per_frame) / ctx->time_base.den;
  AVRational ar;
  ar.num=_frame_rate.den;
  ar.den=_frame_rate.num;
  int64_t dur = av_rescale_q(1, ar, ctx->time_base);
#endif

  frame->setFinished(_frameFinished);
  frame->_pixFormat = ctx->pix_fmt;
  frame->stream_index = packet.packet->stream_index;


  frame->duration = dur;
  _last_pts += dur;
  //  if (!_frameFinished) {
  //    return frame;
  //  }

  frame->pos = 0;
  frame->_type = CODEC_TYPE_VIDEO;
  LOGDEBUG(frame->toString());
  return frame;
}

Frame * Decoder::decodeAudio2(Packet & packet) {
  LOGTRACEMETHOD("Decode Audio");
  LOGDEBUG(packet.toString());
  //        Frame frame;
  int size = packet.packet->size;
  int samples_size = AVCODEC_MAX_AUDIO_FRAME_SIZE;
  int bps = av_get_bits_per_sample_format(ctx->sample_fmt) >> 3;

  uint8_t *outbuf = static_cast<uint8_t*>( av_malloc(samples_size));
  int len = avcodec_decode_audio3(ctx, (short *) outbuf, &samples_size, packet.packet);
  //@TODO: this is a hack, because the decoder changes the TimeBase after the first packet was decoded
  if (_next_pts == AV_NOPTS_VALUE) {
#ifdef USE_TIME_BASE_Q
    _next_pts = av_rescale_q(packet.getPts(), packet.getTimeBase(), AV_TIME_BASE_Q);
#else
    _next_pts = av_rescale_q(packet.getPts(), packet.getTimeBase(), ctx->time_base);
#endif

    LOGDEBUG("setting last pts to " << _next_pts << " ctxtb=" << ctx->time_base.num << "/" << ctx->time_base.den
      << " ptb=" << packet.getTimeBase().num << "/" << packet.getTimeBase().den);
  }
  LOGDEBUG("DecodingLength:" << len << " PacketSize:" << packet.getSize() << "SampleSize:" << samples_size << "FrameSize:" << ctx->frame_size * ctx->channels);
  if (len < 0) {
    LOGERROR("Error while decoding audio Frame");
    return new Frame();
  }
  Frame * frame = new Frame(outbuf, samples_size);

  if (samples_size > 0) {
    frame->setFinished(true);
  } else {
    frame->setFinished(false);
  }
  size -= len;

  frame->_allocated = true;
  //  frame._buffer = outbuf;
  frame->stream_index = packet.packet->stream_index;


  frame->setPts(_next_pts);
  frame->setDts(_next_pts);
  AVRational ar;
  ar.num = 1;
  ar.den = ctx->sample_rate;
#ifdef USE_TIME_BASE_Q
  int64_t dur = av_rescale_q(samples_size, ar, AV_TIME_BASE_Q);
  frame->duration = dur;
  frame->setTimeBase(AV_TIME_BASE_Q);
#else
  //  int64_t dur = av_rescale_q(samples_size, packet.getTimeBase(), ar);
  int64_t dur = ((int64_t) AV_TIME_BASE / bps * samples_size) / (ctx->sample_rate * ctx->channels);
  AVRational arbase;
  arbase.num = 1;
  arbase.den = AV_TIME_BASE;

  frame->duration = av_rescale_q(dur, arbase, ar);
  frame->setTimeBase(ar);

  //  _last_pts += frame->duration;
#endif
  _last_pts = _next_pts;
  _next_pts += frame->duration;
  frame->pos = packet.packet->pos;
  //  frame->duration = packet.packet->duration;
  frame->_size = samples_size;
  frame->_type = CODEC_TYPE_AUDIO;
  frame->channels = ctx->channels;
  frame->sample_rate = ctx->sample_rate;
  LOGDEBUG(frame->toString());
  //  frame->dumpHex();
  return frame;
}

/**
* returns the last Encoded Timestamp
*/
int64_t Decoder::getLastTimeStamp() {
  return _last_pts;
}
