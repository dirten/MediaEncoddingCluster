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
 */
#include "Encoder.h"

#include "Frame.h"
#include "Packet.h"
#include "org/esb/lang/Exception.h"
#include <iostream>



using namespace org::esb::av;
using namespace org::esb;
using namespace std;

Encoder::Encoder(CodecID id) : Codec(id, Codec::ENCODER) {
  _pos = NULL;
  _sink = NULL;
  _last_dts = AV_NOPTS_VALUE;
  _byte_counter = 0;
  _frame_counter = 0;
}

Encoder::Encoder() : Codec(Codec::ENCODER) {
  _pos = NULL;
  _sink = NULL;
  _last_dts = AV_NOPTS_VALUE;
  _byte_counter = 0;
  _frame_counter = 0;
}

Encoder::~Encoder() {
  _last_dts = AV_NOPTS_VALUE;
}

int Encoder::encode(Frame & frame) {
  LOGTRACEMETHOD("Encode Frame");
  _frame_counter++;
  _last_time_base = frame.getTimeBase();
  _last_duration = frame.getDuration();
  _last_idx = frame.stream_index;
  _frames = frame.getFrameCount();
  ctx->sample_aspect_ratio=frame.getPixelAspectRatio();

  if (_last_dts == AV_NOPTS_VALUE) {
    _last_dts = frame.getDts();
    LOGDEBUG("setting last_dts=" << _last_dts);
  }
  if (ctx->codec_type == CODEC_TYPE_VIDEO)
    return encodeVideo(frame);
  if (ctx->codec_type == CODEC_TYPE_AUDIO)
    return encodeAudio(frame);
  LOGERROR("Encoder does not support type:" << ctx->codec_type);
  return -1;
}

int Encoder::encode() {
  _frames = 1;
  return encodeVideo(NULL);
}

/**
 * @TODO: duplicate or drop frames in case of framerate conversion
 * when frame rate will be downscaled, then frames must be dropped
 * when frame rate will be upscaled, then frames must be duplicated
 */
int Encoder::encodeVideo(AVFrame * inframe) {
//  LOGTRACEMETHOD("org.esb.av.Encoder", "encode Video");
  const int buffer_size = 1024 * 256;
  char data[buffer_size];
  memset(&data, 0, buffer_size);
  int frames = 1;
  int ret=0;


  /**
   * calculating the differences between timestamps to duplicate or drop frames
   * when delta < 1.0 then drop a frame
   * when delta > 1.0 then duplicate a frame
   */
  for (int i = 0; i < _frames; i++) {
    if (inframe != NULL)
      inframe->pts = _last_dts;
//    LOGDEBUG("org.esb.av.Encoder", frame.toString());
    ret = avcodec_encode_video(ctx, (uint8_t*) & data, buffer_size, inframe);
    Packet pac(ret);
    if (ret < 0) {
      LOGERROR("Video Encoding failed")
    }

    if (ret > 0) {
      memcpy(pac.packet->data, &data, ret);
      pac.packet->size = ret;
      pac.packet->stream_index = _last_idx;
      if (ctx->coded_frame) {
        if (ctx->coded_frame->key_frame) {
          pac.packet->flags |= PKT_FLAG_KEY;
        }
        pac.packet->pts = ctx->coded_frame->pts;
      }
#ifdef USE_TIME_BASE_Q
      pac.setTimeBase(AV_TIME_BASE_Q);
      pac.setDuration(_last_duration);
#else
      pac.setTimeBase(ctx->time_base);
      pac.setDuration(av_rescale_q(_last_duration, _last_time_base, ctx->time_base));
      pac.setDuration(ctx->ticks_per_frame);
#endif

      pac.packet->dts = _last_dts;
      LOGDEBUG(pac.toString());
      if (_pos != NULL) {
        _pos->writePacket(pac);
      }
      if (_sink != NULL)
        _sink->write(&pac);
    }
    _last_dts += av_rescale_q(_last_duration, _last_time_base, ctx->time_base);
  }
  return ret;
}

int Encoder::encodeVideo(Frame & frame) {
  LOGDEBUG( frame.toString());
  return encodeVideo(frame.getAVFrame());
}

void Encoder::setOutputStream(PacketOutputStream * pos) {
  _pos = pos;
}

void Encoder::setSink(Sink * sink) {
  _sink = sink;
}

int Encoder::encodeAudio(Frame & frame) {

  LOGDEBUG( frame.toString());
  int osize = av_get_bits_per_sample_format(ctx->sample_fmt) / 8;
  int audio_out_size = (4 * 192 * 1024);
  uint8_t * audio_out = static_cast<uint8_t*> (av_malloc(audio_out_size));


  /**
   * when context frame size is > 1 then encode frames in a fifo loop
   */
  if (ctx->frame_size > 1) {
    int frame_bytes = ctx->frame_size * osize * ctx->channels;
    if (av_fifo_realloc2(fifo, av_fifo_size(fifo) + frame._size) < 0) {
      LOGERROR( "av_fifo_realloc2() failed current_size="<<av_fifo_size(fifo) << " adding="<<frame._size);
      //      fprintf(stderr, "av_fifo_realloc2() failed\n");
    }
    av_fifo_generic_write(fifo, frame._buffer, frame._size, NULL);

    int audio_buf_size = (2 * 128 * 1024);
    uint8_t * audio_buf = static_cast<uint8_t*> (av_malloc(audio_buf_size));

    while (av_fifo_size(fifo) >= frame_bytes) {
      av_fifo_generic_read(fifo, audio_buf, frame_bytes, NULL);
      //    uint64_t dur = static_cast<uint64_t> ((((float) frame_bytes / (float) (ctx->channels * osize * ctx->sample_rate)))*((float) 1) / ((float) frame.getTimeBase().num));
      LOGDEBUG( "FrameBytes:" << frame_bytes << ":Channels:" << ctx->channels << ":osize:" << osize << ":sample_rate:" << ctx->sample_rate << "time_base_den:" << ctx->time_base.den);
      int out_size = avcodec_encode_audio(
          ctx,
          audio_out,
          audio_out_size,
          (short*) audio_buf
          );
      if (out_size < 0) {
        LOGERROR( "Error Encoding audio Frame");
      }
      if (out_size == 0) {
        LOGWARN( "out_size=0");
      }

      Packet pak(out_size);
      pak.packet->size = out_size;
      memcpy(pak.packet->data, audio_out, out_size);
      if (ctx->coded_frame) {
        pak.packet->pts = ctx->coded_frame->pts;
      }
      pak.packet->flags |= PKT_FLAG_KEY;
#ifdef USE_TIME_BASE_Q
      pak.setTimeBase(AV_TIME_BASE_Q);
      pak.setDuration(((float) frame_bytes / (float) (ctx->channels * osize * ctx->sample_rate))*(float) 1000000);
#else
      pak.setTimeBase(ctx->time_base);
      pak.setDuration(ctx->frame_size);
#endif
      pak.packet->stream_index = frame.stream_index;
      pak.packet->dts = _last_dts;
      pak.packet->pts = _last_dts;
      _last_dts += pak.getDuration();
      LOGDEBUG( pak.toString());
      if (_pos != NULL)
        _pos->writePacket(pak);
      if (_sink != NULL)
        _sink->write(&pak);
      //      return pak;
    }
    av_free(audio_buf);
  } else {
    int frame_bytes = frame._size;
    const int coded_bps = av_get_bits_per_sample(ctx->codec->id);
    frame_bytes /= osize;
    if (coded_bps)
      frame_bytes = frame_bytes * coded_bps / 8;
    if (frame_bytes > audio_out_size) {
      LOGERROR( "Internal error, buffer size too small");
      //      fprintf(stderr, "Internal error, buffer size too small\n");
      //      exit(1);
    }
    int ret = avcodec_encode_audio(ctx, audio_out, frame_bytes, (short *) frame._buffer);
    if (ret < 0) {
      LOGERROR( "Audio Encoding failed");
      //      fprintf(stderr, "Audio encoding failed\n");
    }
    Packet pak(ret);
    memcpy(pak.packet->data, audio_out, ret);

    pak.packet->flags |= PKT_FLAG_KEY;
#ifdef USE_TIME_BASE_Q
    pak.setTimeBase(AV_TIME_BASE_Q);
    pak.setDuration(((float) frame_bytes / (float) (ctx->channels * osize * ctx->sample_rate))*(float) 1000000);
#else
    pak.setTimeBase(ctx->time_base);
    //      pak.setDuration(ctx->frame_size);
    pak.setDuration((float) frame_bytes / (float) (ctx->channels * osize));
#endif
    pak.packet->stream_index = frame.stream_index;
    pak.packet->dts = _last_dts;
    pak.packet->pts = _last_dts;
    _last_dts += pak.getDuration();
    LOGDEBUG( pak.toString());
    if (_pos != NULL)
      _pos->writePacket(pak);
    if (_sink != NULL)
      _sink->write(&pak);
    //      return pak;
  }
  av_free(audio_out);
  return 0;
}

/**
 * returns the last Encoded Timestamp
 */
int64_t Encoder::getLastTimeStamp() {
  return _last_dts != AV_NOPTS_VALUE ? _last_dts : 0;
}
/**
 * returns the last Encoding Timestamp
 */
int64_t Encoder::getNextTimeStamp() {
  return _last_dts != AV_NOPTS_VALUE ? _last_dts : 0;
}

/**
 * returns the size from the internal fifo Buffer for the Audio Samples
 */
int64_t Encoder::getSamplesBufferd() {
  return av_fifo_size(fifo) / (ctx->channels * 2);
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

