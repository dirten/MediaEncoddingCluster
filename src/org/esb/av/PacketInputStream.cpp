#include "org/esb/lang/Exception.h"
#include "PacketInputStream.h"
#include "Packet.h"
#include "Codec.h"
#include "FormatInputStream.h"
#include "org/esb/io/InputStream.h"
#include "org/esb/lang/Introspec.h"
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
  _fis = NULL;
  if (instanceOf(*is, FormatInputStream)) {
    LOGDEBUG("Instance of FormatInputStream !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!")
    _formatCtx = ((FormatInputStream*) is)->formatCtx;
    _fis = (FormatInputStream*) is;
    _readFrom = 1;

    /**
     * some calculation when the timestamp offset calculation must be done
     * looking for the lowest start time stamp
     * *** this is needed for streams with different start times ***
     */
    int64_t min_start_dts = -1;
    int64_t min_start_pts = -1;
    for (unsigned int a = 0; a < _formatCtx->nb_streams; a++) {

      if(_formatCtx->streams[a]->codec->codec_type!= AVMEDIA_TYPE_AUDIO && _formatCtx->streams[a]->codec->codec_type!=AVMEDIA_TYPE_VIDEO){
        continue;
      }
      if (_formatCtx->streams[a]->first_dts != AV_NOPTS_VALUE && (_formatCtx->streams[a]->first_dts > min_start_dts)) {
        min_start_dts = _formatCtx->streams[a]->first_dts;
      }
      if (_formatCtx->streams[a]->start_time != AV_NOPTS_VALUE && (_formatCtx->streams[a]->start_time > min_start_pts)) {
        min_start_pts = _formatCtx->streams[a]->start_time;
      }
      if (false && !calc) {
        _streams[a].start_dts_offset = 0;
        _streams[a].start_pts_offset = 0;
      }
      _streams[a].discard = trunc;
    }
    if (trunc) {
      std::map<int, StreamData>::iterator it = _streams.begin();
      for (; it != _streams.end(); it++) {
        (*it).second.start_dts_offset = min_start_dts;
        (*it).second.start_pts_offset = min_start_pts;
      }
    }
  } else {
    _source = is;
  }
}

PacketInputStream::~PacketInputStream() {
  av_free_packet(_avpacket);
  av_free(_avpacket);
  if (_readFrom == 1) {
  }
}

/**
 * @deprecated Use int PacketInputStream::readPacket(Packet&packet) instead.
 */
Packet * PacketInputStream::readPacket() {
  return readPacketFromFormatIS();
}

int PacketInputStream::readPacket(Packet&packet) {
  return readPacketFromFormatIS(packet);
}

int PacketInputStream::readPacketFromFormatIS(Packet & packet) {
  int status =0;
  do{
    if (packet.packet->data != NULL){
      av_free_packet(packet.packet);
    }
    status = av_read_frame(_formatCtx, packet.packet);
    if(packet.packet->pts >= _streams[packet.packet->stream_index].start_pts_offset){
      _streams[packet.packet->stream_index].discard=false;
    }
  }while(status >= 0 && _streams[packet.packet->stream_index].discard);

  if (status >= 0) {
    packet.setTimeBase(_formatCtx->streams[packet.getStreamIndex()]->time_base);
    packet.setPtsTimeStamp(TimeStamp(packet.getPts(), Rational(packet.getTimeBase())));
    packet.setDtsTimeStamp(TimeStamp(packet.getDts(), Rational(packet.getTimeBase())));
    packet.setTimeDuration(Duration(packet.getDuration(), Rational(packet.getTimeBase())));

    if(_formatCtx->streams[packet.getStreamIndex()]->parser){
      packet._pict_type=_formatCtx->streams[packet.getStreamIndex()]->parser->pict_type;
    }
  }
  return status;
}

Packet * PacketInputStream::readPacketFromFormatIS() {
  int status=0;
  do{
    if (_avpacket->data != NULL)
      av_free_packet(_avpacket);
    status = av_read_frame(_formatCtx, _avpacket);
    if(_avpacket->pts >= _streams[_avpacket->stream_index].start_pts_offset){
      _streams[_avpacket->stream_index].discard=false;
      //LOGDEBUG("breakpoint")
    }
  //}while(status >= 0 && _avpacket->dts < _streams[_avpacket->stream_index].start_dts_offset);
  }while(status >= 0 && _streams[_avpacket->stream_index].discard);

  if (status >= 0) {
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
