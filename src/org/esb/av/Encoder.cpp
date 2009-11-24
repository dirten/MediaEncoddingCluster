/*----------------------------------------------------------------------
 *  File    : Encoder.cpp
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
 */#include "Encoder.h"

#include "Frame.h"
#include "Packet.h"
#include "org/esb/lang/Exception.h"
#include <iostream>



using namespace org::esb::av;
using namespace org::esb;
using namespace std;

Encoder::Encoder(CodecID id) : Codec(id, Codec::ENCODER) {
  //  fifo = av_fifo_alloc(1024);
  _pos = NULL;
  _sink = NULL;
  _last_dts = AV_NOPTS_VALUE;
  _byte_counter=0;
  _frame_counter=0;
}

Encoder::Encoder() : Codec(Codec::ENCODER) {
  _pos = NULL;
  _sink = NULL;
  _last_dts = AV_NOPTS_VALUE;
 _byte_counter=0;
  _frame_counter=0;
}

Encoder::~Encoder() {
  //  av_fifo_free(fifo);
  _last_dts = AV_NOPTS_VALUE;
}

int Encoder::encode(Frame & frame) {
    _frame_counter++;
  _last_time_base = frame.getTimeBase();
  _last_duration = frame.getDuration();
  _last_idx = frame.stream_index;
  if (_last_dts == AV_NOPTS_VALUE) {
    _last_dts = frame.getDts();
  }
  if (ctx->codec_type == CODEC_TYPE_VIDEO)
    return encodeVideo(frame);
  if (ctx->codec_type == CODEC_TYPE_AUDIO)
    return encodeAudio(frame);
  logerror("Encoder does not support type:" << ctx->codec_type);
  return -1;
}

int Encoder::encode() {
  return encodeVideo(NULL);
}

int Encoder::encodeVideo(AVFrame * inframe) {

  const int buffer_size = 1024 * 256;
  char data[buffer_size];
  memset(&data, 0, buffer_size);

  int ret = avcodec_encode_video(ctx, (uint8_t*) & data, buffer_size, inframe);
  Packet pac(ret);
  //  logdebug("PacketSize:" << ret);
  if (ret > 0) {
    memcpy(pac.packet->data, &data, ret);
  } else {
    //    logdebug("packet size <=0:" << ret);
    return 0;
  }
  //    pac.data=data;

  pac.packet->size = ret;
  //  pac.packet->pts = frame.getPts();
  //  pac.packet->dts = frame.getDts();
  //  pac.packet->pos = frame.pos;
    pac.packet->stream_index = _last_idx;
  //  pac.packet->duration = frame.duration;
  //    pac.flags=0;
  //  logdebug("Encoder data");
  //  frame.toString();
  //  pac.toString();
  if (ctx->coded_frame) {
    //    cout << "!!!!!!!HAVE CODED FRAME!!!!!!!!!!" << endl;
    if (ctx->coded_frame->key_frame) {
      //      logdebug("keyframe!!!");
      pac.packet->flags |= PKT_FLAG_KEY;
    }

    //    pac.packet->pts = av_rescale_q(ctx->coded_frame->pts, ctx->time_base, (AVRational) {1, 25});
    pac.packet->pts = ctx->coded_frame->pts;
  }
#ifdef USE_TIME_BASE_Q
  pac.setTimeBase(AV_TIME_BASE_Q);
  pac.setDuration(_last_duration);
#else
  pac.setTimeBase(ctx->time_base);
  pac.setDuration(av_rescale_q(_last_duration, _last_time_base, ctx->time_base));
#endif

  pac.packet->dts = _last_dts;
  //  pac.packet->dts=_last_dts;
  _last_dts += pac.packet->duration;
  //  _pos->writePacket(pac);
#ifdef DEBUG
  logdebug(pac.toString());
#endif
  if (_pos != NULL) {
    _pos->writePacket(pac);
  }
  if (_sink != NULL)
    _sink->write(&pac);
  return ret;

}

int Encoder::encodeVideo(Frame & frame) {
#ifdef DEBUG
  logdebug(frame.toString());
#endif
  return encodeVideo(frame.getAVFrame());
  //  return Packet();
}

void Encoder::setOutputStream(PacketOutputStream* pos) {
  _pos = pos;
}

void Encoder::setSink(Sink * sink) {
  _sink = sink;
}

int Encoder::encodeAudio(Frame & frame) {
#ifdef DEBUG
  logdebug(frame.toString());
#endif
  int osize = av_get_bits_per_sample_format(ctx->sample_fmt) / 8;

  //  int size_out = frame._size;
  //  uint8_t *fifo_buffer = new uint8_t[frame._size];
  //  memcpy(fifo_buffer, frame._buffer, frame._size);
  int frame_bytes = ctx->frame_size * osize * ctx->channels;
  //  logdebug("FrameSize:" << frame_bytes << " osize:" << osize);
 
  if(frame._size<=_bytes_discard){
//    if(_frame_counter>2)
      _bytes_discard-=frame._size;
    _last_dts +=ctx->frame_size;
    return 0;
  }
  if (av_fifo_realloc2(fifo, av_fifo_size(fifo) + frame._size-_bytes_discard) < 0) {
    fprintf(stderr, "av_fifo_realloc2() failed\n");
  }

  av_fifo_generic_write(fifo, frame._buffer+_bytes_discard, frame._size-_bytes_discard, NULL);
  _bytes_discard=0;

  int audio_buf_size = (2 * 128 * 1024);
  //  uint8_t * audio_buf = new uint8_t[audio_buf_size];
  uint8_t * audio_buf = static_cast<uint8_t*> (av_malloc(audio_buf_size));
  int audio_out_size = (4 * 192 * 1024);
  //  uint8_t * audio_out = new uint8_t[audio_out_size];
  uint8_t * audio_out = static_cast<uint8_t*> (av_malloc(audio_out_size));

  //    logdebug("Fifo size:" << av_fifo_size(fifo) << "FrameBytes:" << frame_bytes);
  while (av_fifo_size(fifo) >= frame_bytes) {
    //#ifdef DEBUG
    //#endif
    av_fifo_generic_read(fifo, audio_buf, frame_bytes, NULL);



    //  const int outbuf_size = 1000;
    //  char outbuf[outbuf_size];
    //    int osize= av_get_bits_per_sample_format(enc->sample_fmt)/8;
    //    logdebug("Frame Size:" << ctx->frame_size << " osize:" << osize);
    //    int64_t dur = av_rescale((int64_t) ctx->frame_size * ctx->time_base.den, ctx->time_base.num, ctx->sample_rate);
    /*
            AVCodecContext *ctx = stream->codec;
        int osize = av_get_bits_per_sample_format(ctx->sample_fmt) / 8;
        int frame_bytes = ctx->frame_size * osize * ctx->channels;
        dur = static_cast<int> ((((float) frame_bytes / (float) (ctx->channels * osize * ctx->sample_rate)))*((float) stream->time_base.den) /((float)stream->time_base.num) );
     */
    //    uint64_t dur = static_cast<uint64_t> ((((float) frame_bytes / (float) (ctx->channels * osize * ctx->sample_rate)))*((float) frame.getTimeBase().den) / ((float) frame.getTimeBase().num));
    uint64_t dur = static_cast<uint64_t> ((((float) frame_bytes / (float) (ctx->channels * osize * ctx->sample_rate)))*((float) 1) / ((float) frame.getTimeBase().num));
    //    uint64_t dur = static_cast<uint64_t>((((float) frame_bytes / (float) (ctx->channels * osize * ctx->sample_rate)))*((float) frame.getTimeBase().den))/frame.getTimeBase().num;
#ifdef DEBUG
    logdebug("FrameBytes:" << frame_bytes << ":Channels:" << ctx->channels << ":osize:" << osize << ":sample_rate:" << ctx->sample_rate << "time_base_den:" << ctx->time_base.den);
#endif
    //    logdebug("!!!!!!!!!!!!!!!!!!!!!!!!!!Duration:" << dur << "::." << (((float) frame_bytes / (float) (ctx->channels * osize * ctx->sample_rate)))*((float) frame.getTimeBase().den));
    //    int64_t dur2=av_rescale_q((int64_t)frame.duration,frame.getTimeBase(),_time_base);
    //    logdebug("Duration:"<<dur2);

    int out_size = avcodec_encode_audio(ctx, audio_out, audio_out_size, (short*) audio_buf);
    //        logdebug("Audio Out Size:" << audio_out_size);
    if (out_size < 0) {
      logerror("Error Encoding audio Frame");
    }
    if (out_size == 0) {
      logwarn("out_size=0");
//      continue;
    }

    Packet pak(out_size);
    pak.packet->size = out_size;
    memcpy(pak.packet->data, audio_out, out_size);
    //    pak.packet->pts = frame.getPts();
    //	pak.pts=this->coded_frame->pts;
    //    pak.setPts(frame.getPts());
    /*
        if (ctx->coded_frame && ctx->coded_frame->pts != AV_NOPTS_VALUE) {
          pak.packet->pts = av_rescale_q(ctx->coded_frame->pts, frame.getTimeBase(), ctx->time_base);
          logdebug("CodedFramePts:" << ctx->coded_frame->pts);
        } else {

        }
     */
    //    else
    //      pak.setPts(frame.getPts());
    //        if(coded_frame && coded_frame->pts != AV_NOPTS_VALUE)
    //    	pak.pts= av_rescale_q(coded_frame->pts, time_base, (AVRational){1,15963});


    if (ctx->coded_frame) {
      pak.packet->pts = ctx->coded_frame->pts;
      //      		cout <<"Encoder Audio Pts:"<<ctx->coded_frame->pts<<endl;
      //	    pak.packet->duration=ctx->coded_frame->duration;
    }
    pak.packet->flags |= PKT_FLAG_KEY;
#ifdef USE_TIME_BASE_Q
    pak.setTimeBase(AV_TIME_BASE_Q);
    pak.setDuration(((float) frame_bytes / (float) (ctx->channels * osize * ctx->sample_rate))*(float) 1000000);
#else
/*
    AVRational ar;
    ar.num = 1;
    ar.den = ctx->sample_rate;*/
    pak.setTimeBase(ctx->time_base);
    pak.setDuration(ctx->frame_size);
#endif
    pak.packet->stream_index = frame.stream_index;
    pak.packet->dts = _last_dts;
    pak.packet->pts = _last_dts;
    _last_dts += pak.getDuration();
    //	pak.packet->pos=frame.pos;
    //    pak.packet->duration = dur;
    //	cout << "FramePts:"<<frame.pts<<"\tEncodedPts"<<pak.pts<<endl;
    //    pak.toString();

#ifdef DEBUG
    logdebug(pak.toString());
#endif
    if (_pos != NULL)
      _pos->writePacket(pak);
    if (_sink != NULL)
      _sink->write(&pak);
    //      return pak;
  }
  av_free(audio_buf);
  av_free(audio_out);
  return 0;
}

/**
 * returns the 2 pass statistics from the last encoded Frame
 */
char * Encoder::getStatistics() {
  if (ctx->stats_out)
    return ctx->stats_out;
  return "no stats\n";
}

/**
 * sets the statistics data for 2 pass encoding
 */
void Encoder::setStatistics(char * stats) {
  ctx->stats_in = stats;
}

