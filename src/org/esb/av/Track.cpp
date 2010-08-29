/* 
 * File:   Track.cpp
 * Author: jholscher
 * 
 * Created on 29. August 2010, 20:05
 */

#include "Track.h"
#include "VideoFormat.h"
#include "AudioFormat.h"
namespace org {
  namespace esb {
    namespace av {

      Track::Track(AVStream * s):_str(s) {

      }


      Track::~Track() {
      }

      Format Track::getFormat() {

        if(_str->codec->codec_type==CODEC_TYPE_VIDEO)
          return VideoFormat(_str->codec->pix_fmt,_str->codec->width,_str->codec->height,Rational(_str->time_base.num,_str->time_base.den));
        if(_str->codec->codec_type==CODEC_TYPE_AUDIO){
          int sample_size=1;
          return AudioFormat(_str->codec->sample_fmt,_str->codec->sample_rate,sample_size, _str->codec->channels,0,_str->codec->frame_size);
        }
        return Format();
      }

      TimeStamp Track::getStartTime() {
        return TimeStamp(_str->first_dts, Rational(_str->time_base.num,_str->time_base.den));
      }

      bool Track::isEnabled() {
      }

      void Track::readFrame(Buffer) {
      }

      void Track::setEnabled(bool) {
      }
    }
  }
}
