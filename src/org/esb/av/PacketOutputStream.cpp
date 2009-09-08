#include "PacketOutputStream.h"
#include "org/esb/io/BufferedOutputStream.h"
#include "org/esb/av/FormatOutputStream.h"
#include "Packet.h"
#include "introspec.h"
#include <stdexcept>
using namespace org::esb::av;
using namespace org::esb::io;

PacketOutputStream::PacketOutputStream(OutputStream * os) {
  if (instanceOf(*os, FormatOutputStream)) {
    _fmtCtx = ((FormatOutputStream*) os)->_fmtCtx;
  } else {
    //	  _target=new BufferedOutputStream(os,32000);
    _target = os; //new BufferedOutputStream(os,32000);
  }
  _fmtCtx->packet_size = 0;
  _fmtCtx->mux_rate = 0;
  _fmtCtx->preload = (int) (0.5 * AV_TIME_BASE);
  _fmtCtx->max_delay = (int) (0.7 * AV_TIME_BASE);
  _fmtCtx->loop_output = AVFMT_NOOUTPUTLOOP;

}

void PacketOutputStream::close() {
  //    av_write_trailer(_fmtCtx);

}

PacketOutputStream::~PacketOutputStream() {
  //    av_write_trailer(_fmtCtx);
  //	delete _target;
}

void PacketOutputStream::writePacket(Packet & packet) {
  if (!_isInitialized)
    throw runtime_error("PacketOutputStream not initialized!!! You must call init() before using writePacket(Packet & packet)");
  if (streams.size() <= packet.getStreamIndex()){
    logerror("there is no stream associated to packet.stream_index #" << packet.getStreamIndex());
    return;
  }

  /**
   * @TOTO implementing now
   * calculate right pts for the entire streams here
   */
  //  streamDts[packet.getStreamIndex()]++;

//  packet.setPts(streamPts[packet.getStreamIndex()]);
//  packet.setDts(streamDts[packet.getStreamIndex()]);
      logdebug("Write Packet to PacketOutputStream"<<packet.toString());
  
//      packet.setDts(AV_NOPTS_VALUE);
//      packet.setPts(AV_NOPTS_VALUE);
//      packet.setDuration(0);

      //  streamDts[packet.getStreamIndex()] += packet.getDuration();
//  streamPts[packet.getStreamIndex()] += packet.getDuration();

  /*
    if (_fmtCtx->streams[packet.getStreamIndex()]->codec->frame_size > 0) {
      streamDts[packet.getStreamIndex()] += _fmtCtx->streams[packet.getStreamIndex()]->codec->frame_size;
      streamPts[packet.getStreamIndex()] += _fmtCtx->streams[packet.getStreamIndex()]->codec->frame_size;
      packet.setDuration(_fmtCtx->streams[packet.getStreamIndex()]->codec->frame_size);
    } else {
      streamDts[packet.getStreamIndex()]++;
      streamPts[packet.getStreamIndex()]++;
      packet.setDuration(0);
    }
   */
  //int result = av_interleaved_write_frame(_fmtCtx, packet.packet);
    int result = av_write_frame(_fmtCtx, packet.packet);
  if(result!=0)logdebug("av_interleaved_write_frame Result:"<<result);
//  logdebug("av_write_frame result:" << result);

}

void PacketOutputStream::setEncoder(Codec & encoder) {
  setEncoder(encoder, 0);
}

void PacketOutputStream::setEncoder(Codec & encoder, int stream_id) {
  //    AVStream * st=av_new_stream(_fmtCtx,_fmtCtx->nb_streams);
  AVStream * st = av_new_stream(_fmtCtx, stream_id);
  if (!st) {
    logerror("Could not alloc stream");
  }
  //  logdebug( "Setting Codec_Id:" << encoder.ctx->codec_id);
  streams.push_back(st);
  st->codec = encoder.ctx;
  st->time_base = encoder.ctx->time_base;
  //  st->time_base = encoder.ctx->time_base;
  return;
  //  	st->time_base.den=90000;


  avcodec_get_context_defaults2(st->codec, encoder.ctx->codec_type);
  //        st->time_base=encoder.getTimeBase();
  //	st->time_base.num=1;
  //	st->time_base.den=90000;


  //  st->stream_copy = 1;
  st->codec->time_base = encoder.ctx->time_base;
  st->codec->codec_type = encoder.ctx->codec_type;
  st->codec->flags = encoder.ctx->flags;

  if (encoder.ctx->codec_type == CODEC_TYPE_AUDIO) {
    st->codec->strict_std_compliance = 0;
    st->codec->channels = encoder.ctx->channels;
    st->codec->sample_rate = encoder.ctx->sample_rate;
    st->codec->sample_fmt = encoder.ctx->sample_fmt;
    AVRational ar; //(1,encoder.ctx->sample_rate);
    ar.num = 1;
    ar.den = encoder.ctx->sample_rate;
    st->codec->time_base = ar; //(AVRational){1,encoder.ctx->sample_rate};
  }
  if (encoder.ctx->codec_type == CODEC_TYPE_VIDEO) {
    st->codec->time_base = encoder.ctx->time_base;

  }


  AVCodecContext * video_enc = st->codec; //=encoder.ctx;

  video_enc->codec_id = encoder.ctx->codec_id;
  video_enc->codec_tag = encoder.ctx->codec_tag;
  video_enc->bit_rate = encoder.ctx->bit_rate;
  video_enc->width = encoder.ctx->width;
  video_enc->height = encoder.ctx->height;
  video_enc->time_base = encoder.ctx->time_base;
  video_enc->pix_fmt = encoder.ctx->pix_fmt;
  video_enc->sample_fmt = encoder.ctx->sample_fmt;
  video_enc->bit_rate_tolerance = 4000 * 1000;


  video_enc->codec_type = encoder.ctx->codec_type;
  video_enc->strict_std_compliance = 0;

  //        video_enc->bit_rate = (int)(acodecBitRate*1000);
  video_enc->sample_rate = encoder.ctx->sample_rate;
  video_enc->channels = encoder.ctx->channels;
  video_enc->frame_size = encoder.ctx->frame_size;
  video_enc->thread_count = 1;


  AVCodec *codec;
  codec = avcodec_find_encoder(encoder.ctx->codec_id);
  if (avcodec_open(st->codec, codec) < 0) {
    fprintf(stderr, "Could not open Codec\n");
  }

  //    st->time_base=st->codec->time_base;
  //  logdebug( "TimeBase #" << stream_id << "\tnum:" << st->time_base.num << "\tden" << st->time_base.den );

  //    av_write_header(_fmtCtx);

}

void PacketOutputStream::init() {
  //  cout << _fmtCtx->oformat->write_header<<endl;
  if (av_write_header(_fmtCtx) < 0) {
    logerror("av_write_header(_fmtCtx) failed");
    exit(1);
  }

  _isInitialized = true;
  dump_format(_fmtCtx, 0, NULL, 1);

  int streams = _fmtCtx->nb_streams;

  for (int a = 0; a < streams; a++) {
    AVStream * stream = _fmtCtx->streams[a];
    //		stream->time_base=stream->codec->time_base;

    logdebug("TimeBase #" << a << "\tnum:" << stream->codec->time_base.num << "\tden" << stream->codec->time_base.den);
    logdebug("TimeBase Stream#" << a << "\tnum:" << stream->time_base.num << "\tden" << stream->time_base.den);
  }

}

/*
void PacketOutputStream::writePacket(Packet * packet){
        write((char*)&packet->pts,sizeof(int64_t));
        write((char*)&packet->dts,sizeof(int64_t));
        write((char*)&packet->size,sizeof(int));
        write((char*)&packet->stream_index,sizeof(int));
        write((char*)&packet->flags,sizeof(int));
        write((char*)&packet->duration,sizeof(int));
        write((char*)&packet->pos,sizeof(int64_t));
        write((char*)packet->data,packet->size);
        flush();
}
 */
void PacketOutputStream::write(char * buffer, int length) {
  _target->write(buffer, length);
}

void PacketOutputStream::write(vector<unsigned char>&buffer) {
  _target->write(buffer);
}

void PacketOutputStream::flush() {
  _target->flush();
}
