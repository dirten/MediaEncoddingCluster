/* 
 * File:   TimeStamp.cpp
 * Author: jholscher
 * 
 * Created on 29. August 2010, 16:45
 */

#include "TimeStamp.h"
namespace org {
  namespace esb {
    namespace av {

      TimeStamp::TimeStamp(int64_t timestamp, Rational base) :
      _timestamp(timestamp),
      _timebase(base) {

      }

      TimeStamp TimeStamp::rescaleTo(Rational r) {
        return TimeStamp(av_rescale_q(_timestamp, _timebase, r),r);
      }

      Rational TimeStamp::getTimeBase() {
        return _timebase;
      }

      bool TimeStamp::operator==(TimeStamp ts) {
        return ts._timestamp==_timestamp&&ts._timebase.num==_timebase.num&&ts._timebase.den==_timebase.den;
      }
        std::string TimeStamp::toString(){
          std::stringstream t;
          t << "TimeStamp=";
        t<<_timestamp;
        return t.str();
      }

      int64_t TimeStamp::getTime() {
        return _timestamp;
      }

      TimeStamp::~TimeStamp() {
      }
    }
  }
}
