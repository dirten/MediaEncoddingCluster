/* 
 * File:   TimeStamp.cpp
 * Author: jholscher
 * 
 * Created on 29. August 2010, 16:45
 */

#include "TimeStamp.h"
#include "org/esb/util/Log.h"
namespace org {
  namespace esb {
    namespace av {

      TimeStamp::TimeStamp(int64_t timestamp, Rational base) :
      _timestamp(timestamp),
      _timebase(base) {

      }

      TimeStamp TimeStamp::rescaleTo(Rational r) {
        return TimeStamp(av_rescale_q(_timestamp, _timebase, r), r);
      }

      Rational TimeStamp::getTimeBase() {
        return _timebase;
      }

      bool TimeStamp::operator==(TimeStamp ts)const {
        return ts._timestamp == _timestamp && av_cmp_q(ts._timebase, _timebase)==0;
      }

      bool TimeStamp::operator!=(TimeStamp ts)const {
        return !(*this==ts);
      }

      bool TimeStamp::operator>(TimeStamp ts)const {
        return av_compare_ts(_timestamp, _timebase,ts._timestamp,ts._timebase)==1;
      }

      bool TimeStamp::operator<(TimeStamp ts)const {
        return av_compare_ts(_timestamp, _timebase,ts._timestamp,ts._timebase)==-1;
      }
      std::string TimeStamp::toString() {
        std::stringstream t;
        t << "TimeStamp=";
        t << _timestamp;
        t << " TimeBase=";
        t<< _timebase.num<<"/"<<_timebase.den;
        return t.str();
      }

      int64_t TimeStamp::getTime() {
        return _timestamp;
      }

      double TimeStamp::toDouble() {
        return av_q2d(_timebase)*_timestamp;
      }

      TimeStamp::~TimeStamp() {
      }
      
      std::ostream& AV_EXPORT operator<<(std::ostream&out,TimeStamp & t){
        out<<t.toString();
        return out;
      }

    }
  }
}
