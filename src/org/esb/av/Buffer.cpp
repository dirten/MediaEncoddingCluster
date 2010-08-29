/* 
 * File:   Buffer.cpp
 * Author: jholscher
 * 
 * Created on 29. August 2010, 17:37
 */

#include "Buffer.h"
namespace org {
  namespace esb {
    namespace av {

      Buffer::Buffer() {
      }

      uint8_t * Buffer::getData() {
        return _data;
      }

      Ptr<AVPacket> Buffer::getAVPacket() {
        return _packet;
      }

      Duration Buffer::getDuration() {
        return _duration;
      }

      int Buffer::getFlags() {
        return _flags;
      }

      const Format Buffer::getFormat() {
        return _format;
      }

      int Buffer::getLength() {
        return _length;
      }

      int64_t Buffer::getSequenceNumber() {
        return _sequenceNumber;
      }

      TimeStamp Buffer::getTimeStamp() {
        return _timeStamp;
      }

      bool Buffer::isDiscard() {
        return _discard;
      }

      bool Buffer::isEOM() {
        return _eom;
      }

      void Buffer::setData(uint8_t*data) {
        _data=data;
      }

      void Buffer::setAVPacket(Ptr<AVPacket> p ) {
        _packet=p;
      }

      void Buffer::setDiscard(bool d) {
        _discard=d;
      }

      void Buffer::setDuration(Duration duration) {
        _duration=duration;
      }

      void Buffer::setEOM(bool eom) {
        _eom=eom;
      }

      void Buffer::setFlags(int flags) {
        _flags=flags;
      }

      void Buffer::setFormat(Format format) {
        _format=format;
      }

      void Buffer::setLength(int length) {
        _length=length;
      }

      void Buffer::setSequenceNumber(int64_t sqnr) {
        _sequenceNumber=sqnr;
      }

      void Buffer::setTimeStamp(TimeStamp ts) {
        _timeStamp=ts;
      }


      Buffer::~Buffer() {
      }
    }
  }
}
