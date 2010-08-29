/* 
 * File:   Demultiplexer.cpp
 * Author: jholscher
 * 
 * Created on 29. August 2010, 20:01
 */

#include "Demultiplexer.h"
namespace org {
  namespace esb {
    namespace av {

      Demultiplexer::Demultiplexer(DataSource& s):_source(s) {

      }

      Duration Demultiplexer::getDuration() {
        return Duration(_source.formatCtx->duration);
      }

      TimeStamp Demultiplexer::getMediaTime() {
        return TimeStamp(_source.formatCtx->timestamp);
      }

      std::list<Track> Demultiplexer::getTracks() {
      }

      bool Demultiplexer::isPositionable() {
        return true;
      }

      bool Demultiplexer::isRandomAccess() {
        return true;
      }

      TimeStamp Demultiplexer::setPosition(TimeStamp t, int r) {
        return av_seek_frame(_source.formatCtx, -1, t.getTime(),r);
      }



      Demultiplexer::~Demultiplexer() {
      }
    }
  }
}
