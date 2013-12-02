#include "PacketOutputStream.h"
#include "org/esb/io/BufferedOutputStream.h"
#include "org/esb/io/File.h"
#include "org/esb/av/FormatOutputStream.h"
#include "Packet.h"
#include "org/esb/util/StringUtil.h"
#include "introspec.h"
#include <stdexcept>
using namespace org::esb::av;
using namespace org::esb::io;
using namespace org::esb::util;

PacketOutputStream::PacketOutputStream(OutputStream * os, std::string statsfile) {
  if (instanceOf(*os, FormatOutputStream)) {
    _fmtCtx = ((FormatOutputStream*) os)->_fmtCtx;
  } else {
    //	  _target=new BufferedOutputStream(os,32000);
    _target = os; //new BufferedOutputStream(os,32000);
  }
  _fmtCtx->packet_size = 0;
  //_fmtCtx->mux_rate = 0;
  //_fmtCtx->preload = (int) (0.5 * AV_TIME_BASE);
  _fmtCtx->max_delay = (int) (0.7 * AV_TIME_BASE);
  //_fmtCtx->loop_output = AVFMT_NOOUTPUTLOOP;
  _isInitialized = false;
  _stats_fos = NULL;
  if (statsfile.length() > 0) {
    File sfile(statsfile);
    _stats_fos = new org::esb::io::FileOutputStream(&sfile);
  }
}

bool PacketOutputStream::close() {
  bool result = false;
  if (_isInitialized) {

    list<AVStream*>::iterator it = streams.begin();

    for (; it != streams.end(); it++) {
      if ((*it)->codec == NULL) {
        LOGERROR("Codecs are closed, this can result in an unknown behavior!");
      }
    }

    LOGINFO("writing trailer");
    result = av_write_trailer(_fmtCtx) == 0;

    it = streams.begin();
    for (; it != streams.end(); it++) {
      av_free((*it));
    }

    if (_stats_fos) {
      delete _stats_fos;
      _stats_fos = NULL;
    }
  }

  _isInitialized = false;
  return result;
}

PacketOutputStream::~PacketOutputStream() {
  LOGDEBUG("PacketOutputStream::~PacketOutputStream()");
  close();
}
bool first_packet = true;

void PacketOutputStream::newPacket(Packet * p){
  writePacket(*p);
}
void PacketOutputStream::newPacket(AVPacket * p){
  writePacket(p);
}

int PacketOutputStream::writePacket(Packet & packet) {
  writePacket(packet.getAVPacket());
}

int PacketOutputStream::writePacket(AVPacket * packet) {
  if (!_isInitialized) {
    if (first_packet)
      LOGERROR("PacketOutputStream not initialized!!! You must call init() before using writePacket(Packet & packet)");
    return -1;
  }
  if (streams.size() <= packet->stream_index) {
    LOGERROR("there is no stream associated to packet.stream_index #" << packet->stream_index);
    return -1;
  }

  /**
   * @TODO implementing now
   * calculate right pts for the entire streams here
   */
  //  streamDts[packet.getStreamIndex()]++;

  //  packet.setPts(streamPts[packet.getStreamIndex()]);
  //  packet.setDts(streamDts[packet.getStreamIndex()]);
  //      logdebug("Write Packet to PacketOutputStream"<<packet.toString());

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

  /*calculating the Duration of a Packet*/
  /*
  int dur = 0;
  
  if (stream->codec->codec_type == CODEC_TYPE_VIDEO) {
    dur = static_cast<int> ((((float) (stream->time_base.den) / (float) (stream->time_base.den)))*((float) stream->codec->time_base.den) / ((float) stream->codec->time_base.num));
  } else
    if (stream->codec->codec_type == CODEC_TYPE_AUDIO) {
    AVCodecContext *ctx = stream->codec;
    int osize = av_get_bits_per_sample_format(ctx->sample_fmt) / 8;
    int frame_bytes = ctx->frame_size * osize * ctx->channels;
    dur = static_cast<int> ((((float) frame_bytes / (float) (ctx->channels * osize * ctx->sample_rate)))*((float) stream->time_base.den) / ((float) stream->time_base.num));
  } else {
    logdebug("CodecType unknown");
  }*/
  //LOGDEBUG(packet.toString());
  AVStream *stream = _fmtCtx->streams[packet->stream_index];
  if(packet->duration==0)
    packet->duration=1;
  packet->duration=av_rescale_q(packet->duration, stream->codec->time_base, stream->time_base);
  packet->pts=av_rescale_q(packet->pts, stream->codec->time_base, stream->time_base);
  //packet.setDts(av_rescale_q(packet.getDts(), packet.getTimeBase(), stream->time_base));


  /*calculating the dts*/
  //  streamDts[packet.getStreamIndex()]+=dur;
  //  packet.packet->pts=AV_NOPTS_VALUE;
  //  packet.setDuration(0);
  packet->dts=AV_NOPTS_VALUE;
  //packet.setDts(0);
  //  packet.setPts(streamDts[packet.getStreamIndex()]);

  //LOGDEBUG(packet.toString());
  //  compute_pkt_fields2(_fmtCtx->streams[packet.getStreamIndex()], packet.packet);
  //  logdebug(packet.toString());
  //uint8_t dur = static_cast<uint8_t>((((float) frame_bytes / (float) (ctx->channels * osize * ctx->sample_rate)))*((float) frame.getTimeBase().den))/frame.getTimeBase().num;
  if (false && first_packet && packet->stream_index == 0) {
    //    logdebug("writing first packet");
    first_packet = false;
    AVPacket p;
    av_init_packet(&p);
    p.stream_index = 0;
    p.pts = 1;
    p.dts = 1;
    p.size = 0;
    p.data = NULL;
    int result = av_write_frame(_fmtCtx, &p);
    //    if (result != 0)logdebug("av_interleaved_write_frame Result:" << result);
  }
  //int result =_fmtCtx->oformat->write_packet(_fmtCtx,packet.packet);
  LOGDEBUG("write packet to stream"<<packet);
  int result = av_interleaved_write_frame(_fmtCtx, packet);
  /*
  if (_fmtCtx->streams[packet->stream_index]->codec->codec_type == AVMEDIA_TYPE_VIDEO && _stats_fos) {
    _stats_fos->write(StringUtil::toString(packet._quality) + ":" + StringUtil::toString(packet.packet->size) + ",");
  }*/
  //  int result = av_write_frame(_fmtCtx, packet.packet);
  if (result != 0) {
    LOGERROR("av_interleaved_write_frame Result:" << result);
    LOGERROR(packet);
  }
  //LOGDEBUG(packet.toString());
  first_packet = false;
  return result;
}

void PacketOutputStream::setEncoder(Codec & encoder) {
  setEncoder(encoder, 0);
}

std::list<AVStream*> PacketOutputStream::getStreamList() {
  return streams;
}

void PacketOutputStream::setEncoder(Codec & encoder, int stream_id) {
  //    AVStream * st=av_new_stream(_fmtCtx,_fmtCtx->nb_streams);
  AVStream * st = avformat_new_stream(_fmtCtx, encoder._codec);
  if (!st) {
    LOGERROR("Could not alloc stream");
  }

  //  logdebug( "Setting Codec_Id:" << encoder.ctx->codec_id);
  streams.push_back(st);
  /*freeing allocated codec from av_new_stream
   because we will using our own codec context*/
  av_free(st->codec);
  st->codec = encoder.ctx;
  //st->time_base = encoder.ctx->time_base;
  st->sample_aspect_ratio = encoder.ctx->sample_aspect_ratio;
  //  st->time_base = encoder.ctx->time_base;
  return;
  //  	st->time_base.den=90000;


  //avcodec_get_context_defaults2(st->codec, encoder.ctx->codec_type);
  //        st->time_base=encoder.getTimeBase();
  //	st->time_base.num=1;
  //	st->time_base.den=90000;


  //  st->stream_copy = 1;
  st->codec->time_base = encoder.ctx->time_base;
  st->codec->codec_type = encoder.ctx->codec_type;
  st->codec->flags = encoder.ctx->flags;

  if (encoder.ctx->codec_type == AVMEDIA_TYPE_AUDIO) {
    st->codec->strict_std_compliance = 0;
    st->codec->channels = encoder.ctx->channels;
    st->codec->sample_rate = encoder.ctx->sample_rate;
    st->codec->sample_fmt = encoder.ctx->sample_fmt;
    AVRational ar; //(1,encoder.ctx->sample_rate);
    ar.num = 1;
    ar.den = encoder.ctx->sample_rate;
    st->codec->time_base = ar; //(AVRational){1,encoder.ctx->sample_rate};
  }
  if (encoder.ctx->codec_type == AVMEDIA_TYPE_VIDEO) {
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

/*  if (avcodec_open(st->codec, codec) < 0) {
    fprintf(stderr, "Could not open Codec\n");
  }*/

  //    st->time_base=st->codec->time_base;
  //  logdebug( "TimeBase #" << stream_id << "\tnum:" << st->time_base.num << "\tden" << st->time_base.den );

  //    av_write_header(_fmtCtx);

}

bool PacketOutputStream::init() {
  //  cout << _fmtCtx->oformat->write_header<<endl;
  if (avformat_write_header(_fmtCtx, NULL) < 0) {
    LOGERROR("av_write_header(_fmtCtx) failed");
    return false;
    //    exit(1);
  }

  _isInitialized = true;
  av_dump_format(_fmtCtx, 0, NULL, 1);

  int streams = _fmtCtx->nb_streams;

  for (int a = 0; a < streams; a++) {
    AVStream * stream = _fmtCtx->streams[a];
    //		stream->time_base=stream->codec->time_base;

    LOGDEBUG("TimeBase #" << a << "\tnum:" << stream->codec->time_base.num << "\tden" << stream->codec->time_base.den);
    LOGDEBUG("TimeBase Stream#" << a << "\tnum:" << stream->time_base.num << "\tden" << stream->time_base.den);
    if (stream->parser)
      LOGDEBUG("Stream repeast_pict:" << stream->parser->repeat_pict);
  }
  return true;
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
