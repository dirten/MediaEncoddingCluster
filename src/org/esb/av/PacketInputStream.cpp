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
  //  _video_idx = -1;
  //  _audio_idx = -1;
  _fis = NULL;
  if (instanceOf(*is, FormatInputStream)) {
    _formatCtx = ((FormatInputStream*) is)->formatCtx;
    _fis = (FormatInputStream*) is;
    _readFrom = 1;

    /**
     * some calculation when the timestamp offset calculation must be done
     * looking for the lowest start time stamp
     */
    int64_t min_start_dts = -1;
    int64_t min_start_pts = -1;
    for (int a = 0; a < _formatCtx->nb_streams; a++) {
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
  if (_readFrom == 1) {
    //        if(_packet.data!=NULL)
    //	    av_free_packet(&_packet);
  }
}

/**
 * @deprecated Use int PacketInputStream::readPacket(Packet&packet) instead.
 */
Packet PacketInputStream::readPacket() {
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
//    av_dup_packet(packet.packet);
    packet.setTimeBase(_formatCtx->streams[packet.getStreamIndex()]->time_base);
    /*
        if (_fis->_streamReverseMap[packet.getStreamIndex()]>-1)
          packet.setStreamIndex(_fis->_streamReverseMap[packet.getStreamIndex()]);
        else
          status = readPacketFromFormatIS(packet);
     */
  }
  return status;
}

Packet PacketInputStream::readPacketFromFormatIS() {
  Packet pac;
  //    av_init_packet(&pac);
  //        if(_packet.data!=NULL)
  //            av_free_packet(&_packet);
  av_read_frame(_formatCtx, pac.packet);
  logdebug("Packet Size:" << pac.packet->size)
  return pac;
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
  return _formatCtx->file_size;
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
