#include "org/esb/lang/Exception.h"
#include "PacketInputStream.h"
#include "Packet.h"
#include "Codec.h"
#include "FormatInputStream.h"
#include "org/esb/io/InputStream.h"
#include "introspec.h"
#include <iostream>

using namespace std;
using namespace org::esb::av;
using namespace org::esb::lang;

/**
 * the PacketInputStream reads currently only the first Video and Audio Stream
 * and maps this Streams to the Stream index
 * Video always StreamIndex 0
 * Audio always StreamIndex 1 if video stream exist else then the Stream Index is 0
 **/
PacketInputStream::PacketInputStream(InputStream * is, bool trunc, bool calc) {
  _readFrom = 0;
  _avpacket=static_cast<AVPacket*>(av_malloc(sizeof(AVPacket)));
    av_init_packet(_avpacket);

  //  _video_idx = -1;
  //  _audio_idx = -1;
  _fis = NULL;
  if (instanceOf(*is, FormatInputStream)) {
    LOGDEBUG("Instance of FormatInputStream !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!")
    _formatCtx = ((FormatInputStream*) is)->formatCtx;
    _fis = (FormatInputStream*) is;
    _readFrom = 1;

    /**
     * some calculation when the timestamp offset calculation must be done
     * looking for the lowest start time stamp
     */
    int64_t min_start_dts = -1;
    int64_t min_start_pts = -1;
    for (unsigned int a = 0; a < _formatCtx->nb_streams; a++) {
//      if(_formatCtx->streams[a]->parser)
//        _formatCtx->streams[a]->parser->flags|=PARSER_FLAG_COMPLETE_FRAMES;

      if (_formatCtx->streams[a]->first_dts != AV_NOPTS_VALUE && (min_start_dts == -1 || _formatCtx->streams[a]->first_dts < min_start_dts)) {
        min_start_dts = _formatCtx->streams[a]->first_dts;
      }
      if (_formatCtx->streams[a]->first_dts != AV_NOPTS_VALUE && (min_start_pts == -1 || _formatCtx->streams[a]->start_time < min_start_pts)) {
        min_start_pts = _formatCtx->streams[a]->start_time;
      }
      if (!calc) {
        _streams[a].start_dts_offset = 0;
        _streams[a].start_pts_offset = 0;
      }
//      _streams[a].start_dts_offset = av_rescale_q(_formatCtx->start_time, AV_TIME_BASE_Q,_formatCtx->streams[a]->time_base);
//      _streams[a].start_pts_offset = av_rescale_q(_formatCtx->start_time, AV_TIME_BASE_Q,_formatCtx->streams[a]->time_base);

      _streams[a].discard = trunc;
    }
    /*
        if (calc) {
          std::map<int, StreamData>::iterator it = _streams.begin();
          for (; it != _streams.end(); it++) {
            (*it).second.start_dts_offset = av_rescale_q(_formatCtx->start_time,AV_TIME_BASE_Q);
            (*it).second.start_pts_offset = _formatCtx->start_time;
          }
        }*/
  } else {
    _source = is;
  }
}

PacketInputStream::~PacketInputStream() {
  av_free(_avpacket);
  if (_readFrom == 1) {
    //        if(_packet.data!=NULL)
    	    
  }
}

/**
 * @deprecated Use int PacketInputStream::readPacket(Packet&packet) instead.
 */
Packet * PacketInputStream::readPacket() {
  //    if(_readFrom==1)
  return readPacketFromFormatIS();
  //    return readPacketFromIS();

}

int PacketInputStream::readPacket(Packet&packet) {
  return readPacketFromFormatIS(packet);
}

int PacketInputStream::readPacketFromFormatIS(Packet & packet) {
  if (packet.packet->data != NULL)
    av_free_packet(packet.packet);
  int status = av_read_frame(_formatCtx, packet.packet);

  if (status >= 0) {
    //LOGDEBUG("DecodeStreamQuality:"<<_formatCtx->streams[packet.getStreamIndex()]->quality*100.0f);
//    av_dup_packet(packet.packet);
    packet.setTimeBase(_formatCtx->streams[packet.getStreamIndex()]->time_base);

    packet.setPtsTimeStamp(TimeStamp(packet.getPts(), Rational(packet.getTimeBase())));
    packet.setDtsTimeStamp(TimeStamp(packet.getDts(), Rational(packet.getTimeBase())));
    packet.setTimeDuration(Duration(packet.getDuration(), Rational(packet.getTimeBase())));

    if(_formatCtx->streams[packet.getStreamIndex()]->parser){
      packet._pict_type=_formatCtx->streams[packet.getStreamIndex()]->parser->pict_type;
    }
    /*
        if (_fis->_streamReverseMap[packet.getStreamIndex()]>-1)
          packet.setStreamIndex(_fis->_streamReverseMap[packet.getStreamIndex()]);
        else
          status = readPacketFromFormatIS(packet);
     */
  }
  return status;
}

Packet * PacketInputStream::readPacketFromFormatIS() {
  if (av_read_frame(_formatCtx, _avpacket) >= 0) {
    Packet *pac=new Packet(_avpacket);
    pac->setTimeBase(_formatCtx->streams[pac->getStreamIndex()]->time_base);
    pac->setPtsTimeStamp(TimeStamp(pac->getPts(), Rational(pac->getTimeBase())));
    pac->setDtsTimeStamp(TimeStamp(pac->getDts(), Rational(pac->getTimeBase())));
    pac->setTimeDuration(Duration(pac->getDuration(), Rational(pac->getTimeBase())));
    if(_formatCtx->streams[pac->getStreamIndex()]->parser){
      pac->_pict_type=_formatCtx->streams[pac->getStreamIndex()]->parser->pict_type;
    }
//    logdebug("Packet Size:" << pac->getSize())
    return pac;
  }
//  delete pac;
  return NULL;
}

int PacketInputStream::read(unsigned char * buffer, int length) {
  return _source->read(buffer, length);
}

int PacketInputStream::read() {
  return _source->read();
}

int PacketInputStream::read(vector<unsigned char>&buffer) {
  return _source->read(buffer);
}

long long int PacketInputStream::available(bool isBlocking) {
  return 0;//_formatCtx->file_size;
}

uint64_t PacketInputStream::getDuration() {
  return _formatCtx->streams[_streamIndex]->duration;
}

Codec * PacketInputStream::getCodec() {
  return _codec2;
}

void PacketInputStream::skip(long packets) {
  av_seek_frame(_formatCtx, _streamIndex, packets, AVSEEK_FLAG_ANY);
}
