/* 
 * File:   Track.cpp
 * Author: jholscher
 * 
 * Created on 29. August 2010, 20:05
 */

#include "Track.h"
#include "VideoFormat.h"
#include "AudioFormat.h"
#include "Demultiplexer.h"
namespace org {
  namespace esb {
    namespace av {

      Track::Track(AVStream * s, Demultiplexer *plexer):_str(s),_plexer(plexer) {
        _enabled=true;
        _seq=0;
      }

      Track::~Track() {
      }

      Format Track::getFormat() {

        if(_str->codec->codec_type==CODEC_TYPE_VIDEO)
          return VideoFormat(_str->codec);
        if(_str->codec->codec_type==CODEC_TYPE_AUDIO){
          return AudioFormat(_str->codec);
        }
        return Format();
      }

      TimeStamp Track::getStartTime() {
        return TimeStamp(_str->first_dts, Rational(_str->time_base.num,_str->time_base.den));
      }
      
      int Track::getStreamIndex(){
        return _str->index;
      }

      bool Track::isEnabled() {
        return _enabled;
      }

      void Track::readFrame(Buffer & buf) {
        if(!_enabled)return;
        _plexer->readFrame(buf,_str->index);
        buf.setSequenceNumber(_seq++);
        buf.setFormat(getFormat());
      }

      void Track::setEnabled(bool en) {
        _enabled=en;
        if(en==false){
          _str->discard=AVDISCARD_ALL;
        }else{
          _str->discard=AVDISCARD_NONE;
        }
      }
    }
  }
}
