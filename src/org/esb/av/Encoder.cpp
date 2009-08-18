#include "Encoder.h"

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
}

Encoder::Encoder() : Codec() {


}

Encoder::~Encoder() {
//  av_fifo_free(fifo);
}

Packet Encoder::encode(Frame & frame) {
  if (ctx->codec_type == CODEC_TYPE_VIDEO)
    return encodeVideo(frame);
  if (ctx->codec_type == CODEC_TYPE_AUDIO)
    return encodeAudio(frame);
  return Packet();
}

Packet Encoder::encodeVideo(Frame & frame) {
  //  logdebug("VideoEncoderFrame");
  //  frame.toString();
  const int buffer_size = 1024 * 256;
  char data[buffer_size];
  memset(&data, 0, buffer_size);

  int ret = avcodec_encode_video(ctx, (uint8_t*) & data, buffer_size, frame.getAVFrame());
  //    pac.data=new uint8_t[ret];
  //	cout << "ret:"<<ret<<endl;
  //    logdebug("\rCreate Packet with Size:"<<ret);
  Packet pac(ret);
  if (ret > 0) {
    memcpy(pac.packet->data, &data, ret);
    //      delete []data;
  } else {
    throw lang::Exception();
  }
  //    pac.data=data;
  pac.setTimeBase(frame.getTimeBase());
  pac.packet->size = ret;
  pac.packet->pts = frame.getPts();
  pac.packet->dts = frame.getDts();
  pac.packet->pos = frame.pos;
  pac.packet->stream_index = frame.stream_index;
  pac.packet->duration = frame.duration;
  //    pac.flags=0;
  logdebug("Encoder data");
  frame.toString();
  pac.toString();
  if (ctx->coded_frame) {
    //		cout <<"!!!!!!!HAVE CODED FRAME!!!!!!!!!!"<<endl;
    if (ctx->coded_frame->key_frame)
      pac.packet->flags |= PKT_FLAG_KEY;
    //		pac.packet->pts= av_rescale_q(ctx->coded_frame->pts, ctx->time_base, (AVRational){1,25});
    //		pac.packet->pts=ctx->coded_frame->pts;
  }
  //  _pos->writePacket(pac);
  if (_sink != NULL)
    _sink->write(&pac);
  return pac;
}

void Encoder::setOutputStream(PacketOutputStream* pos) {
  _pos = pos;
}

void Encoder::setSink(Sink * sink) {
  _sink = sink;
}

Packet Encoder::encodeAudio(Frame & frame) {
  //  logdebug("AudioEncoderFrame");
  //  frame.toString();

  int osize = av_get_bits_per_sample_format(ctx->sample_fmt) / 8;

  int size_out = frame._size * ctx->channels * osize;

  int frame_bytes = ctx->frame_size * osize * ctx->channels;

  if (av_fifo_realloc2(fifo, av_fifo_size(fifo) + size_out) < 0) {
    fprintf(stderr, "av_fifo_realloc2() failed\n");
  }

  av_fifo_generic_write(fifo, frame._buffer, size_out, NULL);


  int audio_buf_size = (2 * 128 * 1024);
  uint8_t * audio_buf = new uint8_t[audio_buf_size];
  int audio_out_size = (4 * 128 * 1024);
  uint8_t * audio_out = new uint8_t[audio_out_size];

  while (av_fifo_size(fifo) >= frame_bytes) {

    av_fifo_generic_read(fifo, audio_buf, frame_bytes, NULL);



    //  const int outbuf_size = 1000;
    //  char outbuf[outbuf_size];
    //    int osize= av_get_bits_per_sample_format(enc->sample_fmt)/8;
    logdebug("Frame Size:" << ctx->frame_size << " osize:" << osize);
    int64_t dur = av_rescale((int64_t) ctx->frame_size * _time_base.den, _time_base.num, ctx->sample_rate);
    //    int64_t dur2=av_rescale_q((int64_t)frame.duration,frame.getTimeBase(),_time_base);
    //    logdebug("Duration:"<<dur2);
    int out_size = avcodec_encode_audio(ctx, audio_out, audio_out_size, (short*) audio_buf);
    logdebug("Audio Out Size:" << audio_out_size);
    if (out_size < 0) {
      logerror("Error Encoding audio Frame");
    }

    Packet pak(out_size);
    pak.packet->size = out_size;
    memcpy(pak.packet->data, audio_out, out_size);
    //    pak.packet->pts = frame.getPts();
    //	pak.pts=this->coded_frame->pts;

    if (ctx->coded_frame && ctx->coded_frame->pts != AV_NOPTS_VALUE) {
      pak.packet->pts = ctx->coded_frame->pts; //av_rescale_q(ctx->coded_frame->pts, ctx->time_base, (AVRational) {1, 48000});
      logdebug("CodedFramePts:" << ctx->coded_frame->pts);
    }
    //    else
    //      pak.setPts(frame.getPts());
    //        if(coded_frame && coded_frame->pts != AV_NOPTS_VALUE)
    //    	pak.pts= av_rescale_q(coded_frame->pts, time_base, (AVRational){1,15963});


    if (ctx->coded_frame) {
      //		pak.packet->pts=ctx->coded_frame->pts;
      //		cout <<"Encoder Audio Pts:"<<ctx->coded_frame->pts<<endl;
      //	    pak.packet->duration=ctx->coded_frame->duration;
    }
    pak.setTimeBase(frame.getTimeBase());
    pak.packet->flags |= PKT_FLAG_KEY;
    pak.packet->stream_index = frame.stream_index;

    pak.packet->dts = frame.getDts();
    //	pak.packet->pos=frame.pos;
    pak.packet->duration = dur;
    //	cout << "FramePts:"<<frame.pts<<"\tEncodedPts"<<pak.pts<<endl;
    pak.toString();
//    if (_pos != NULL)
//      _pos->writePacket(pak);
    if (_sink != NULL)
      _sink->write(&pak);
//      return pak;
  }
  delete []audio_buf;
  delete []audio_out;
  return Packet();
}

/**
 * returns the 2 pass statistics from the last encoded Frame
 */
char * Encoder::getStatistics() {
  if (ctx->stats_out)
    return ctx->stats_out;
  return NULL;
}

/**
 * sets the statistics data for 2 pass encoding
 */
void Encoder::setStatistics(char * stats) {
  ctx->stats_in = stats;
}

