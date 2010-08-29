/* 
 * File:   TimeStamp.h
 * Author: jholscher
 *
 * Created on 29. August 2010, 16:45
 */

#ifndef TIMESTAMP_H
#define	TIMESTAMP_H
#include "Rational.h"
namespace org {
  namespace esb {
    namespace av {

      class TimeStamp {
      public:
        TimeStamp(int64_t timestamp=1, Rational base=Rational());
        virtual ~TimeStamp();
        TimeStamp rescaleTo(Rational );
        Rational getTimeBase();
        int64_t getTime();
        bool operator==(TimeStamp t);
        std::string toString();
      private:
        int64_t _timestamp;
        Rational _timebase;
      };
    }
  }
}

#endif	/* TIMESTAMP_H */

