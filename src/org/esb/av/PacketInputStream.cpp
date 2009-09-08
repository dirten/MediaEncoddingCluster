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
PacketInputStream::PacketInputStream(InputStream * is) {
  _readFrom = 0;
  //  _video_idx = -1;
  //  _audio_idx = -1;
  _fis = NULL;
  if (instanceOf(*is, FormatInputStream)) {
    _formatCtx = ((FormatInputStream*) is)->formatCtx;
    _fis = (FormatInputStream*) is;
    
    
    _readFrom = 1;
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
 * @deprecated Use the software.
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
  if ( status == 0) {
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
