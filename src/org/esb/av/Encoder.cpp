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
#include "org/esb/lang/Ptr.h"
#include "org/esb/lang/Exception.h"
#include "org/esb/util/StringUtil.h"
#include <iostream>



using namespace org::esb::av;
using namespace org::esb;
using namespace std;
using org::esb::util::StringUtil;
Encoder::Encoder(CodecID id) : Codec(id, Codec::ENCODER) {
  _pos = NULL;
  _sink = NULL;
  _last_dts = AV_NOPTS_VALUE;
  _byte_counter = 0;
  _frame_counter = 0;
}
Encoder::Encoder(std::string name) : Codec(name, Codec::ENCODER) {
  _pos = NULL;
  _sink = NULL;
  _last_dts = AV_NOPTS_VALUE;
  _byte_counter = 0;
  _frame_counter = 0;
}

Encoder::Encoder(std::map<std::string, std::string> data): Codec(data, Codec::ENCODER)
{
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

bool Encoder::open() {
  ctx->channel_layout=AV_CH_LAYOUT_STEREO;
  bool result = Codec::open();
  if (false && result && getCodecType() == AVMEDIA_TYPE_VIDEO && (ctx->flags & CODEC_FLAG_PSNR)) {
    /*initialize the refDecoder*/
    std::map<std::string, std::string>opt = getCodecOptions();
    _refDecoder = new Decoder(getCodecId());
    std::map<std::string, std::string>::iterator opit = opt.begin();
    for (; opit != opt.end(); opit++) {
      if ((*opit).first != "extradata" || (*opit).first != "extradata_size")
        _refDecoder->setCodecOption((*opit).first, (*opit).second);
    }
    _refDecoder->setWidth(getWidth());
    _refDecoder->setHeight(getHeight());
    _refDecoder->setPixelFormat((PixelFormat) 0);
    LOGDEBUG("EncoderExtrdataSize:" << ctx->extradata_size);
    LOGDEBUG("RefDecoderExtrdataSize:" << ctx->extradata_size);
    _refDecoder->ctx->extradata = static_cast<uint8_t*> (av_malloc(ctx->extradata_size));
    memcpy(_refDecoder->ctx->extradata, ctx->extradata, ctx->extradata_size);
    if (_refDecoder->open()) {
      LOGDEBUG("Reference Decoder openned" << _refDecoder->toString());
    }
  }
  return result;
}

bool Encoder::newFrame(Ptr<Frame> f){
  if(f){
    return encode(*f);
  }else{
    if(getCodecType() == AVMEDIA_TYPE_VIDEO){
      return encode();
    }else{
      LOGDEBUG("flush audio")
        return encodeAudio2(NULL);
    }
  }
}

int Encoder::encode(Frame & frame) {
  LOGTRACEMETHOD("Encode");
  _actualFrame = &frame;
  _frame_counter++;
  _last_time_base = frame.getTimeBase();
  _last_duration = frame.getDuration();
  _last_idx = frame.stream_index;
  _frames = frame.getFrameCount();
  ctx->sample_aspect_ratio = frame.getPixelAspectRatio();
  LOGDEBUG("Encode Frame:"<<frame.toString())
  /*handling to flush the encoder*/
  if(!frame.isFinished()){
    LOGDEBUG("flushing encoder")
    emptyFrameIsEOF=true;
  }
  if (_last_dts == AV_NOPTS_VALUE) {
    _last_dts = 0;//frame.getPts();
    LOGDEBUG("setting last_dts=" << _last_dts);
  }
  if (ctx->codec_type == AVMEDIA_TYPE_VIDEO) {
    LOGTRACEMETHOD("Encode Video");

    return encodeVideo(frame);
  }
  if (ctx->codec_type == AVMEDIA_TYPE_AUDIO) {
    LOGTRACEMETHOD("Encode Audio");
    return encodeAudio2(frame);
  }
  LOGERROR("Encoder does not support type:" << ctx->codec_type);
  return -1;
}

int Encoder::encodeVideo(Frame & frame) {
  //LOGDEBUG(frame.toString());
  if (false && !_pix_fmt_converter) {
    _input_format.width = ctx->width;
    _input_format.height = ctx->height;
    _input_format.pixel_format = STD_PIX_FMT;
    Format out = _input_format;
    out.pixel_format = ctx->pix_fmt;
    _pix_fmt_converter = new PixelFormatConverter(_input_format, out);
    _pix_fmt_converter->open();
  }
  //Ptr<Frame> tmp_frame = new Frame(ctx->pix_fmt, ctx->width, ctx->height);
  //_pix_fmt_converter->process(frame,*tmp_frame);
  LOGDEBUG(frame.toString());

  /*setting the right pts for the input to the codec*/
  frame.setPts(av_rescale_q(frame.getPts(),frame.getTimeBase(),ctx->time_base));
  frame.getAVFrame()->pict_type=AV_PICTURE_TYPE_NONE;
  return encodeVideo2(frame.getAVFrame());
  //  return encodeVideo(frame.getAVFrame());
}

int Encoder::encode() {
  _frames = 1;
  return encodeVideo2(NULL);
}

/**
 * @TODO: duplicate or drop frames in case of framerate conversion
 * when frame rate will be downscaled, then frames must be dropped
 * when frame rate will be upscaled, then frames must be duplicated
 */
int Encoder::encodeVideo(AVFrame * inframe) {
  //  LOGTRACEMETHOD("org.esb.av.Encoder", "encode Video");
  //  int buffer_size = 1024 * 256;
  int size = ctx->width * ctx->height;
  const int buffer_size = FFMAX(1024 * 256, 6 * size + 200);

  char * data = new char[buffer_size];
  memset(data, 0, buffer_size);
  int frames = 1;
  int ret = 0, got_output;


  /**
   * calculating the differences between timestamps to duplicate or drop frames
   * when delta < 1.0 then drop a frame
   * when delta > 1.0 then duplicate a frame
   */
  for (int i = 0; i < _frames; i++) {
    //if (inframe != NULL)
    //  inframe->pts = _last_dts;
    //    LOGDEBUG("org.esb.av.Encoder", frame.toString());
    if(false){
      Ptr<Packet> pacInt=new Packet();
      Packet pac=*pacInt.get();
      ret = avcodec_encode_video2(ctx, pac.getAVPacket(), inframe, &got_output);
    }
    ret = avcodec_encode_video(ctx, (uint8_t*) data, buffer_size, inframe);
    Packet pac(ret);
    if (ret < 0) {
      LOGERROR("Video Encoding failed")
    }
    if (ret >= 0) {
      LOGDEBUG("Frame encoded");
      if (ctx->coded_frame && ctx->coded_frame->quality > 0){
        LOGDEBUG("EnCodedFrameQuality:" << ctx->coded_frame->quality / (float) FF_QP2LAMBDA);
        pac._quality=ctx->coded_frame->quality / (float) FF_QP2LAMBDA;
      }
      if (ctx->stats_out)
        LOGDEBUG("stats available:" << ctx->stats_out);
      memcpy(pac.packet->data, data, ret);
      pac.packet->size = ret;
      pac.packet->stream_index = _stream_index;
      if (ctx->coded_frame) {
        if (ctx->coded_frame->key_frame) {
          pac.packet->flags |= AV_PKT_FLAG_KEY;
        }
        pac.packet->pts = ctx->coded_frame->pts;
        pac.setPtsTimeStamp(TimeStamp(ctx->coded_frame->pts,ctx->time_base));
        pac._pict_type=ctx->coded_frame->pict_type;
      }
#ifdef USE_TIME_BASE_Q
      pac.setTimeBase(AV_TIME_BASE_Q);
      pac.setDuration(_last_duration);
#else
      pac.setTimeBase(ctx->time_base);
      if (_last_duration > 0 && _last_time_base.num > 0 && _last_time_base.den > 0) {
        pac.setDuration(av_rescale_q(_last_duration, _last_time_base, ctx->time_base));
      } else {
        LOGERROR("frame has no duration and no timebase, could not recalculate the duration of the encoded packet!!!");
      }
      //pac.setDuration(ctx->ticks_per_frame);
#endif

      pac.packet->dts = _last_dts;
      pac.setDtsTimeStamp(TimeStamp(_last_dts, ctx->time_base));
      LOGDEBUG(pac.toString());
      if (ctx->flags & CODEC_FLAG_PSNR) {
        LOGDEBUG("ERROR0========" << ctx->coded_frame->error[0]);
        LOGDEBUG("ERROR1========" << ctx->coded_frame->error[1]);
        LOGDEBUG("ERROR2========" << ctx->coded_frame->error[2]);
        LOGDEBUG("ERROR3========" << ctx->coded_frame->error[3]);
      }
      LOGDEBUG("PSNR=========" << ((double) ctx->coded_frame->error[0]) / ((double) ctx->width * ctx->height * 255.0 * 255.0));
      /**
       * calculate the psnr here
       */
      if (_refDecoder) {
        Frame * tmpf = _refDecoder->decode2(pac);
        if (tmpf->isFinished()) {
          LOGDEBUG("Reference Frame Decoded:" << tmpf->toString());
          processPsnr(_actualFrame, tmpf);
        }
        delete tmpf;
      }

      pushPacket(Ptr<Packet>(new Packet(pac)));
      //pushPacket(pacInt);

      if (_pos != NULL) {
        _pos->writePacket(pac);
      }
      if (_sink != NULL)
        _sink->write(&pac);
    }
    if (_last_duration > 0 && _last_time_base.num > 0 && _last_time_base.den > 0)
      _last_dts += av_rescale_q(_last_duration, _last_time_base, ctx->time_base);
  }
  delete [] data;


  return ret;
}
int Encoder::encodeVideo2(AVFrame * inframe) {
  int ret = 0, got_output;
  _frames=1;


  Ptr<Packet> pacInt=Ptr<Packet>(new Packet());
  Packet *pac=pacInt.get();
  ret = avcodec_encode_video2(ctx, pac->getAVPacket(), inframe, &got_output);

  if (ret < 0) {
    LOGERROR("Video Encoding failed in int Encoder::encodeVideo2(AVFrame * inframe) ")
  }

  if (ret >= 0) {

    if (ctx->coded_frame && ctx->coded_frame->quality > 0){
      pac->_quality=ctx->coded_frame->quality / (float) FF_QP2LAMBDA;
    }

    pac->packet->stream_index = _stream_index;
    if (ctx->coded_frame) {
      if (ctx->coded_frame->key_frame) {
        pac->packet->flags |= AV_PKT_FLAG_KEY;
      }
      //pac->packet->pts = ctx->coded_frame->pts;
      pac->setPtsTimeStamp(TimeStamp(ctx->coded_frame->pts,ctx->time_base));
      pac->_pict_type=ctx->coded_frame->pict_type;
    }
#ifdef USE_TIME_BASE_Q
    pac->setTimeBase(AV_TIME_BASE_Q);
    pac->setDuration(_last_duration);
#else
    pac->setTimeBase(ctx->time_base);
    if (_last_duration > 0 && _last_time_base.num > 0 && _last_time_base.den > 0) {
      pac->setDuration(av_rescale_q(_last_duration, _last_time_base, ctx->time_base));
    } else {
      LOGERROR("frame has no duration and no timebase, could not recalculate the duration of the encoded packet!!!");
    }
#endif

    //pac->packet->dts = _last_dts;
    //pac->packet->pts = _last_dts;
    pac->setDtsTimeStamp(TimeStamp(_last_dts, ctx->time_base));
    //LOGDEBUG(pac->toString());

    //LOGDEBUG("PSNR=========" << ((double) ctx->coded_frame->error[0]) / ((double) ctx->width * ctx->height * 255.0 * 255.0));
    /**
       * calculate the psnr here
       */
    if (_refDecoder) {
      Frame * tmpf = _refDecoder->decode2(*pac);
      if (tmpf->isFinished()) {
        LOGDEBUG("Reference Frame Decoded:" << tmpf->toString());
        processPsnr(_actualFrame, tmpf);
      }
      delete tmpf;
    }

    //pushPacket(Ptr<Packet>(new Packet(pac)));
    if(got_output){
      LOGDEBUG("push packet:"<<pacInt->toString());
      //pushPacket(Ptr<Packet>(new Packet()));
      return pushPacket(pacInt);
    }else{
      /*@TODO: needed to flush packets here*/
    }
  }
  if (_last_duration > 0 && _last_time_base.num > 0 && _last_time_base.den > 0){
    _last_dts += av_rescale_q(_last_duration, _last_time_base, ctx->time_base);
  }
  //LOGDEBUG("encoder ret:"<<ret)
  return ret;
}


void Encoder::setOutputStream(PacketOutputStream * pos) {
  _pos = pos;
}

void Encoder::setSink(Sink * sink) {
  _sink = sink;
}
int Encoder::encodeAudio2(Frame & frame) {
  encodeAudio2(frame.getAVFrame());
}

int Encoder::encodeAudio2(AVFrame * frame) {
  // LOGDEBUG("EncodeAudio2:"<<frame.toString());
  Ptr<Packet> pak=new Packet();
  int got_packet;
  int out_size = avcodec_encode_audio2(
        ctx,
        pak->getAVPacket(),
        frame,
        &got_packet
        );

  pak->setTimeBase(ctx->time_base);
  pak->setDuration(ctx->frame_size);
  pak->packet->stream_index = _stream_index;

  pak->packet->dts = _last_dts;
  pak->packet->pts = _last_dts;
  if (ctx->coded_frame) {
    pak->setDts(ctx->coded_frame->pts);
    pak->setPts(ctx->coded_frame->pts);
  }
  _last_dts += pak->getDuration();
  LOGDEBUG("EncodeAudio2:"<<pak->toString());
  if(got_packet)
    return pushPacket(pak);
  return false;
  if (_pos != NULL)
    _pos->writePacket(*pak);
  if (_sink != NULL)
    _sink->write(pak.get());
}


int Encoder::encodeAudio(Frame & frame) {

  LOGDEBUG(frame.toString());
  int osize = av_get_bits_per_sample_fmt(ctx->sample_fmt) / 8;
  LOGDEBUG("bits per sample format:" << osize << " fmt:" << ctx->sample_fmt)
      int audio_out_size = (4 * 192 * 1024);
  uint8_t * audio_out = static_cast<uint8_t*> (av_malloc(audio_out_size));
  //uint8_t * audio_out = new uint8_t[audio_out_size];//static_cast<uint8_t*> (av_malloc(audio_out_size));


  /**
   * when context frame size is > 1 then encode frames in a fifo loop
   */
  if (ctx->frame_size > 1) {
    int frame_bytes = ctx->frame_size * osize * ctx->channels;
    if (av_fifo_realloc2(fifo, av_fifo_size(fifo) + frame._size) < 0) {
      LOGERROR("av_fifo_realloc2() failed current_size=" << av_fifo_size(fifo) << " adding=" << frame._size);
      //      fprintf(stderr, "av_fifo_realloc2() failed\n");
    }
    LOGDEBUG("fifo size:"<< av_fifo_size(fifo)<<" fifo space:"<<av_fifo_space(fifo))
        int bytes=av_fifo_generic_write(fifo, frame._buffer, frame._size, NULL);
    if(bytes<=0 && bytes!=frame._size){
      throw new Exception("fifo not correctly filles up -> buffer_size="+StringUtil::toString(frame._size)+" bytes reded from fifo="+StringUtil::toString(bytes));
    }
    int audio_buf_size = (2 * 128 * 1024);
    uint8_t * audio_buf = static_cast<uint8_t*> (av_malloc(audio_buf_size));
    //uint8_t * audio_buf = new uint8_t[audio_buf_size];

    while (av_fifo_size(fifo) >= frame_bytes) {
      av_fifo_generic_read(fifo, audio_buf, frame_bytes, NULL);
      //    uint64_t dur = static_cast<uint64_t> ((((float) frame_bytes / (float) (ctx->channels * osize * ctx->sample_rate)))*((float) 1) / ((float) frame.getTimeBase().num));
      LOGDEBUG("FrameBytes:" << frame_bytes << ":Channels:" << ctx->channels << ":osize:" << osize << ":sample_rate:" << ctx->sample_rate << "time_base_den:" << ctx->time_base.den);
      int out_size = avcodec_encode_audio(
            ctx,
            audio_out,
            audio_out_size,
            (short*) audio_buf
            );
      if (out_size < 0) {
        LOGERROR("Error Encoding audio Frame");
      }
      if (out_size == 0) {
        continue;
        //LOGWARN("out_size=0");
      }
      if (out_size > 0 && ctx->coded_frame && ctx->coded_frame->pts != AV_NOPTS_VALUE) {
        LOGDEBUG("Encoded Audio Frame PTS:" << ctx->coded_frame->pts);

      }
      Packet pak(out_size);
      pak.packet->size = out_size;
      memcpy(pak.packet->data, audio_out, out_size);
      if (ctx->coded_frame) {
        pak.packet->pts = ctx->coded_frame->pts;
      }
      pak.packet->flags |= AV_PKT_FLAG_KEY;
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
      if (ctx->coded_frame) {
        pak.setDts(ctx->coded_frame->pts);
        pak.setPts(ctx->coded_frame->pts);
      }
      _last_dts += pak.getDuration();
      LOGDEBUG(pak.toString());
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
      LOGERROR("Internal error, buffer size too small");
      //      fprintf(stderr, "Internal error, buffer size too small\n");
      //      exit(1);
    }
    int ret = avcodec_encode_audio(ctx, audio_out, frame_bytes, (short *) frame._buffer);
    if (ret < 0) {
      LOGERROR("Audio Encoding failed");
      //      fprintf(stderr, "Audio encoding failed\n");
    }
    Packet pak(ret);
    memcpy(pak.packet->data, audio_out, ret);

    pak.packet->flags |= AV_PKT_FLAG_KEY;
#ifdef USE_TIME_BASE_Q
    pak.setTimeBase(AV_TIME_BASE_Q);
    pak.setDuration(((float) frame_bytes / (float) (ctx->channels * osize * ctx->sample_rate))*(float) 1000000);
#else
    pak.setTimeBase(ctx->time_base);
    //      pak.setDuration(ctx->frame_size);
    pak.setDuration((float) frame_bytes / (float) (ctx->channels * osize));
#endif
    //pak.packet->stream_index = frame.stream_index;
    pak.packet->stream_index = _stream_index;
    pak.packet->dts = _last_dts;
    pak.packet->pts = _last_dts;
    _last_dts += pak.getDuration();
    LOGDEBUG(pak.toString());
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
  return const_cast<char*> ("no stats\n");
}

/**
 * sets the statistics data for 2 pass encoding
 */
void Encoder::setStatistics(char * stats) {
  ctx->stats_in = stats;
}

void Encoder::processPsnr(Frame * ref, Frame * cmp) {

}

void Encoder::setPassLogfile(std::string file) {
  _passlogfile = file;
}

std::string Encoder::getPassLogfile() {
  return _passlogfile;
}

void Encoder::writeStatistics(std::string data) {

}

std::string Encoder::readStatistics() {
  std::string result;
  return result;
}
