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
#include "Format.h"
#include <iostream>
#include <stdexcept>
#include "org/esb/util/Log.h"



using namespace org::esb::av;
using namespace std;

Decoder::Decoder() : Codec(Codec::DECODER) {
  _last_pts = AV_NOPTS_VALUE;
  _next_pts = AV_NOPTS_VALUE;
  frame = Ptr<Frame>(new Frame());
  _input_packet_time_base.num=0;
  _input_packet_time_base.den=0;
}

Decoder::Decoder(CodecID id) : Codec(id, Codec::DECODER) {
  _last_pts = AV_NOPTS_VALUE;
  _next_pts = AV_NOPTS_VALUE;
  frame = Ptr<Frame>(new Frame());
  _input_packet_time_base.num=0;
  _input_packet_time_base.den=0;
}

Decoder::Decoder(std::string name) : Codec(name, Codec::DECODER) {
  _last_pts = AV_NOPTS_VALUE;
  _next_pts = AV_NOPTS_VALUE;
  frame = Ptr<Frame>(new Frame());
  _input_packet_time_base.num=0;
  _input_packet_time_base.den=0;
}

Decoder::Decoder(AVStream * c) : Codec(c, Codec::DECODER) {
  _last_pts = AV_NOPTS_VALUE;
  _next_pts = AV_NOPTS_VALUE;
  frame = Ptr<Frame>(new Frame());
  _input_packet_time_base.num=0;
  _input_packet_time_base.den=0;
}

Decoder::~Decoder() {
}

void Decoder::reset(){
  emptyFrameIsEOF=false;
  Codec::reset();
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
  frame._type = AVMEDIA_TYPE_VIDEO;
  return frame;

}

bool Decoder::newPacket(Ptr<Packet> p){
  return decode3(*p);
}

Frame * Decoder::decode2(Packet & packet) {
  if (!_opened)
    throw runtime_error("Codec not opened");
  packet.setStreamIndex(_stream_index);
  if (ctx->codec_type == AVMEDIA_TYPE_VIDEO){
    return decodeVideo2(packet);
  }
  if (ctx->codec_type == AVMEDIA_TYPE_AUDIO){
    return decodeAudio2(packet);
  }

  throw runtime_error("Packet is no type of Video or Audio");
  //  return Frame ();
}

bool Decoder::decode3(Packet & packet) {
  if (!_opened)
    throw runtime_error("Codec not opened");
  packet.setStreamIndex(_stream_index);
  /*this is needed to temporary save the input time_base, it is needed to flush the packets at the end of the stream*/
  if(_input_packet_time_base.num==0 && _input_packet_time_base.den==0){
    _input_packet_time_base=packet.getTimeBase();
  }

  if (ctx->codec_type == AVMEDIA_TYPE_VIDEO){
    bool result=decodeVideo3(packet);
    return result;
  }

  if (ctx->codec_type == AVMEDIA_TYPE_AUDIO){
    bool result=decodeAudio3(packet);
    return result;
  }

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
  if (false && !_pix_fmt_converter) {
    Format in;
    in.width = ctx->width;
    in.height = ctx->height;
    in.pixel_format = ctx->pix_fmt;
    _output_format = in;
    _output_format.pixel_format = STD_PIX_FMT;
    _pix_fmt_converter = new PixelFormatConverter(in, _output_format);
    _pix_fmt_converter->open();
  }
  //Ptr<Frame> tmp_frame = new Frame(ctx->pix_fmt, ctx->width, ctx->height, false);
  //Frame * frame = new Frame(ctx->pix_fmt, ctx->width, ctx->height);
  Frame * frame = new Frame();
  int _frameFinished = 0;
  int len = packet.packet->size;
  LOGDEBUG(packet.toString());

  //  while (len > 0) {
  //    logdebug("Decode Packet");
  int bytesDecoded = 0;
  //if (ctx->codec_id > -1) {
  bytesDecoded = avcodec_decode_video2(ctx, frame->getAVFrame(), &_frameFinished, packet.packet);
  //}
  if (_frameFinished) {

    //_pix_fmt_converter->process(*tmp_frame, *frame);
    if (ctx->coded_frame) {
      LOGDEBUG("DeCodedFrameQuality:" << ctx->coded_frame->quality);
      LOGDEBUG("Interlaced:" << ctx->coded_frame->interlaced_frame);
      LOGDEBUG("topfieldfirst:" << ctx->coded_frame->top_field_first);
      //LOGDEBUG("PictureType:" << av_get_pict_type_char(ctx->coded_frame->pict_type));
    }
  }
  //@TODO: this is a hack, because the decoder changes the TimeBase after the first packet was decoded
  if (false && _last_pts == AV_NOPTS_VALUE) {
#ifdef USE_TIME_BASE_Q
    _last_pts = av_rescale_q(packet.getPts(), packet.getTimeBase(), AV_TIME_BASE_Q);
#else
    _last_pts = av_rescale_q(packet.getPts(), packet.getTimeBase(), _frame_rate);
#endif
    LOGDEBUG("setting last pts to " << _last_pts << " ctxtb=" << _frame_rate.num << "/" << _frame_rate.den
    << " ptb=" << packet.getTimeBase().num << "/" << packet.getTimeBase().den);
  }

  LOGDEBUG("BytesDecoded:" << bytesDecoded);
  frame->setPixelAspectRatio(ctx->sample_aspect_ratio);
  LOGDEBUG("PAR " << ctx->sample_aspect_ratio.num << "/" << ctx->sample_aspect_ratio.den);
  LOGDEBUG("RES " << ctx->coded_width << "/" << ctx->coded_height);
  AVRational display_aspect_ratio;
  av_reduce(&display_aspect_ratio.num, &display_aspect_ratio.den,
  ctx->width * ctx->sample_aspect_ratio.num,
  ctx->height * ctx->sample_aspect_ratio.den,
  1024 * 1024);
  LOGDEBUG("DAR " << display_aspect_ratio.num << "/" << display_aspect_ratio.den);

  if (bytesDecoded < 0) {
    LOGERROR("Error while decoding frame");
  }
  /**
   * if frame is not finished, returns the blank frame
   * the calling process of decode must ensure to check if the returning frame isFinished by calling the Method isFinished()
   */
  if (!_frameFinished) {
    LOGDEBUG("frame not finished");
    return frame;
  }
  frame->setStorageAspectRatio(ctx->coded_width, ctx->coded_height);
  frame->setPixelAspectRatio(ctx->sample_aspect_ratio);
  frame->setDisplayAspectRatio(display_aspect_ratio);
  len -= bytesDecoded;


  //  }
  /* calculating the Presentation TimeStamp here*/
  frame->setPts(_last_pts);
  //frame->setDts(_last_pts);

#ifdef USE_TIME_BASE_Q
  frame->setTimeBase(AV_TIME_BASE_Q);
  // calculating the duration of the decoded packet
  int64_t dur = av_rescale_q(packet.packet->duration, packet.getTimeBase(), AV_TIME_BASE_Q);
#else
  //  frame->setTimeBase(ctx->time_base);
  // calculating the duration of the decoded packet
  //    int64_t dur = av_rescale_q(packet.packet->duration, packet.getTimeBase(), ctx->time_base);
  //  int64_t tmp_dur=((int64_t)AV_TIME_BASE * ctx->time_base.num * ctx->ticks_per_frame) / ctx->time_base.den;
  AVRational ar;
  ar.num = _frame_rate.den;
  ar.den = _frame_rate.num; //* ctx->ticks_per_frame;
  LOGDEBUG("ticks:" << ctx->ticks_per_frame);
  //  int64_t dur = av_rescale_q(ar.num , ar, ctx->time_base);
  frame->setTimeBase(ar);
  int64_t dur = 1;
#endif

  frame->setFinished(_frameFinished!=0);
  //  frame->_pixFormat = ctx->pix_fmt;
  frame->stream_index = _stream_index;//packet.packet->stream_index;


  frame->duration = dur;
  _last_pts += dur;
  //  if (!_frameFinished) {
  //    return frame;
  //  }

  frame->pos = 0;
  frame->_type = AVMEDIA_TYPE_VIDEO;
  LOGDEBUG(frame->toString());
  pushFrame(new Frame(*frame));
  return frame;
}

bool Decoder::decodeVideo3(Packet & packet) {
  int _frameFinished = 0;
  int len = packet.packet->size;
  LOGDEBUG("decode input video packet:"<<packet.toString());

  int bytesDecoded = 0;

  bytesDecoded = avcodec_decode_video2(ctx, frame->getAVFrame(), &_frameFinished, packet.packet);

  if (_frameFinished) {
    if (ctx->coded_frame) {

      LOGDEBUG("DeCodedFrameQuality:" << ctx->coded_frame->quality);
      LOGDEBUG("Interlaced:" << ctx->coded_frame->interlaced_frame);
      LOGDEBUG("topfieldfirst:" << ctx->coded_frame->top_field_first);
      //LOGDEBUG("PictureType:" << av_get_pict_type_char(ctx->coded_frame->pict_type));
      LOGDEBUG("PicturePts:" << frame->getAVFrame()->best_effort_timestamp);
    }
  }
  //@TODO: this is a hack, because the decoder changes the TimeBase after the first packet was decoded
  if (_last_pts == AV_NOPTS_VALUE) {
#ifdef USE_TIME_BASE_Q
    _last_pts = av_rescale_q(packet.getPts(), packet.getTimeBase(), AV_TIME_BASE_Q);
#else
    AVRational fr;
    fr.num = _frame_rate.den;
    fr.den = _frame_rate.num;
    _last_pts = 0;// av_rescale_q(packet.getPts(), packet.getTimeBase(), fr);
#endif
    LOGDEBUG("setting last pts to " << _last_pts << " ctxtb=" << ctx->time_base.num << "/" << ctx->time_base.den
    << " ptb=" << packet.getTimeBase().num << "/" << packet.getTimeBase().den);
  }

  AVRational display_aspect_ratio;
  av_reduce(&display_aspect_ratio.num, &display_aspect_ratio.den, ctx->width * ctx->sample_aspect_ratio.num, ctx->height * ctx->sample_aspect_ratio.den,  1024 * 1024 );

  frame->setPixelAspectRatio(ctx->sample_aspect_ratio);
  frame->setStorageAspectRatio(ctx->coded_width, ctx->coded_height);
  frame->setPixelAspectRatio(ctx->sample_aspect_ratio);
  frame->setDisplayAspectRatio(display_aspect_ratio);


  if (bytesDecoded < 0) {
    LOGERROR("Error while decoding frame");
  }

  if(packet.packet->size==0)
    emptyFrameIsEOF=true;

  if (!_frameFinished) {
    if(emptyFrameIsEOF){
      LOGDEBUG("Decoder flushed");
      return pushFrame(Ptr<Frame>());
    }
    /*eof not reached, continue cosuming packets*/
    return true;
  }
  len -= bytesDecoded;


  //  }
  /* calculating the Presentation TimeStamp here*/
  frame->setPts(_last_pts);
  //frame->setDts(_last_pts);

#ifdef USE_TIME_BASE_Q
  frame->setTimeBase(AV_TIME_BASE_Q);
  // calculating the duration of the decoded packet
  int64_t dur = av_rescale_q(packet.packet->duration, packet.getTimeBase(), AV_TIME_BASE_Q);
#else
  //  frame->setTimeBase(ctx->time_base);
  // calculating the duration of the decoded packet
  //    int64_t dur = av_rescale_q(packet.packet->duration, packet.getTimeBase(), ctx->time_base);
  //  int64_t tmp_dur=((int64_t)AV_TIME_BASE * ctx->time_base.num * ctx->ticks_per_frame) / ctx->time_base.den;
  AVRational ar;
  ar.num = _frame_rate.den;
  ar.den = _frame_rate.num; //* ctx->ticks_per_frame;
  //LOGDEBUG("ticks:" << ctx->ticks_per_frame);
  frame->setTimeBase(ctx->time_base);
  int64_t dur = av_rescale_q(frame->getAVFrame()->pkt_duration , _input_packet_time_base, ctx->time_base);
  //int64_t dur = ctx->time_base.num*ctx->ticks_per_frame;
#endif

  frame->setFinished(_frameFinished!=0);
  //  frame->_pixFormat = ctx->pix_fmt;
  frame->stream_index = _stream_index;//packet.packet->stream_index;


  frame->duration = dur;
  _last_pts += dur;
  //  if (!_frameFinished) {
  //    return frame;
  //  }

  frame->pos = 0;
  frame->_type = AVMEDIA_TYPE_VIDEO;
  LOGDEBUG("Push Video Frame from decoder:"<<frame->toString());
  return pushFrame(frame);
  //return true;
}

Frame * Decoder::decodeAudio2(Packet & packet) {
  LOGTRACEMETHOD("Decode Audio");
  LOGDEBUG(packet.toString());
  //        Frame frame;
  Frame * frame = new Frame();
  int size = packet.packet->size;
  int samples_size = 192000;//AVCODEC_MAX_AUDIO_FRAME_SIZE;
  int bps = av_get_bits_per_sample_fmt(ctx->sample_fmt) >> 3;
  //uint8_t* t=(uint8_t*)av_malloc(100);
  //uint8_t *outbuf = static_cast<uint8_t*> (av_malloc(samples_size));
  //int len = avcodec_decode_audio3(ctx, (short *) outbuf, &samples_size, packet.packet);
  int len = avcodec_decode_audio4(ctx, frame->getAVFrame(), &samples_size, packet.packet);

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
  if (len < 0 /*||ctx->channels<=0||samples_size<=0*/) {
    LOGERROR("Error while decoding audio Frame");
    //av_free(outbuf);
    return new Frame();
  }
  //Frame * frame = new Frame(outbuf,samples_size);
  if (samples_size > 0) {
    frame->setFinished(true);
  } else {
    frame->setFinished(false);
  }
  size -= len;

  frame->_allocated = true;
  //frame->getAVFrame()->nb_samples=samples_size;
  //  frame._buffer = outbuf;
  frame->stream_index = packet.packet->stream_index;


  frame->setPts(_next_pts);
  //frame->setDts(_next_pts);
  AVRational ar;
  ar.num = 1;
  ar.den = ctx->sample_rate;
#ifdef USE_TIME_BASE_Q
  int64_t dur = av_rescale_q(samples_size, ar, AV_TIME_BASE_Q);
  frame->duration = dur;
  frame->setTimeBase(AV_TIME_BASE_Q);
#else
  //  int64_t dur = av_rescale_q(samples_size, packet.getTimeBase(), ar);
  //samples_size=max(1,samples_size);
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
  /*
  frame->_size = samples_size;
  frame->_buffer=outbuf;
  */
  frame->_type = AVMEDIA_TYPE_AUDIO;
  frame->channels = ctx->channels;
  frame->sample_rate = ctx->sample_rate;
  //frame->dumpHex();
  LOGDEBUG(frame->toString());
  //  frame->dumpHex();
  pushFrame(new Frame(*frame));

  return frame;
}

bool Decoder::decodeAudio3(Packet & packet) {
  LOGDEBUG("audio decoder input:"<<packet.toString());
  int samples_size = 0;//AVCODEC_MAX_AUDIO_FRAME_SIZE;
  //int bps = av_get_bits_per_sample_fmt(ctx->sample_fmt) >> 3;
  static int ch=0;
  if(ch==0){
    LOGDEBUG("Context channels:"<<ctx->channels);
    //ch=ctx->channels;
    char ch_name[1000];
    av_get_channel_layout_string(ch_name, 1000, 0,ctx->channel_layout);
    LOGDEBUG("Channel Layout:"<<ch_name)
  }
  avcodec_get_frame_defaults(frame->getAVFrame());
  //frame->getAVFrame()->channel_layout=0x3;
  int bps = av_get_bytes_per_sample(ctx->sample_fmt);
  //uint8_t* t=(uint8_t*)av_malloc(100);
  //uint8_t *outbuf = static_cast<uint8_t*> (av_malloc(samples_size));
  //int len = avcodec_decode_audio3(ctx, (short *) outbuf, &samples_size, packet.packet);
  //LOGDEBUG("decoder before ch layout = "<<ctx->channel_layout)
  LOGDEBUG("decode input audio packet:"<<packet.toString());

  int len = avcodec_decode_audio4(ctx, frame->getAVFrame(), &samples_size, packet.packet);
  LOGDEBUG("decoder ch layout = "<<ctx->channel_layout)
  char ch_name[1000];
  av_get_channel_layout_string(ch_name, 1000, 0,ctx->channel_layout);
  LOGDEBUG("Channel Layout:"<<ch_name)
  if(packet.packet->size==0)
  emptyFrameIsEOF=true;

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
  if (len < 0 /*||ctx->channels<=0||samples_size<=0*/) {
    LOGERROR("Error while decoding audio Frame");
    //av_free(outbuf);
    return false;
  }
  //Frame * frame = new Frame(outbuf,samples_size);
  if (samples_size > 0) {
    frame->setFinished(true);
  } else {
    frame->setFinished(false);
    if(emptyFrameIsEOF)
      return pushFrame(new Frame());
  }
  //size -= len;

  frame->_allocated = true;
  //frame->getAVFrame()->nb_samples=samples_size;
  //  frame._buffer = outbuf;
  frame->stream_index = packet.packet->stream_index;


  frame->setPts(_next_pts);
  //frame->setDts(_next_pts);
  AVRational ar;
  ar.num = 1;
  ar.den = ctx->sample_rate;
#ifdef USE_TIME_BASE_Q
  int64_t dur = av_rescale_q(samples_size, ar, AV_TIME_BASE_Q);
  frame->duration = dur;
  frame->setTimeBase(AV_TIME_BASE_Q);
#else
  //  int64_t dur = av_rescale_q(samples_size, packet.getTimeBase(), ar);
  //samples_size=max(1,samples_size);
  int64_t dur = ((int64_t) AV_TIME_BASE / bps * ctx->frame_size) / (ctx->sample_rate * ctx->channels);
  AVRational arbase;
  arbase.num = 1;
  arbase.den = AV_TIME_BASE;
  frame->duration = frame->getAVFrame()->nb_samples;//av_rescale_q(dur, arbase, ar);
  frame->setTimeBase(ar);

  //  _last_pts += frame->duration;
#endif
  _last_pts = _next_pts;
  _next_pts += frame->duration;
  frame->pos = packet.packet->pos;
  //  frame->duration = packet.packet->duration;
  /*
  frame->_size = samples_size;
  frame->_buffer=outbuf;
  */
  frame->_type = AVMEDIA_TYPE_AUDIO;
  //frame->getAVFrame()->channels = ch;//ctx->channels;
  frame->sample_rate = ctx->sample_rate;
  //frame->dumpHex();
  LOGDEBUG("Push Audio Frame from decoder:"<<frame->toString());
  //LOGDEBUG(frame->toString());
  //  frame->dumpHex();
  if(samples_size>0 && len>0)
    return pushFrame(frame);

  return !emptyFrameIsEOF;
}

/**
 * returns the last Encoded Timestamp
 */
int64_t Decoder::getLastTimeStamp() {
  return _last_pts;
}

std::string Decoder::getStaticCodecName(CodecID codec_id) {
  std::string result;
  AVCodec *p = NULL;
  int a = 0;
  while ((p = av_codec_next(p))) {
    if (p->decode && p->id == codec_id) {
      result=p->name;
    }
  }
  return result;
}

CodecID Decoder::getStaticCodecId(std::string codec_name) {
  CodecID result;
  AVCodec *p = NULL;
  int a = 0;
  while ((p = av_codec_next(p))) {
    if (p->decode && p->name != NULL&&p->name ==codec_name) {
      result=p->id;
    }
  }
  return result;

}

