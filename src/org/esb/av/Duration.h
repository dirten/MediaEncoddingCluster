/* 
 * File:   Duration.h
 * Author: jholscher
 *
 * Created on 29. August 2010, 18:15
 */

#ifndef DURATION_H
#define	DURATION_H
#include "TimeStamp.h"
namespace org {
  namespace esb {
    namespace av {

      class Duration:public TimeStamp {
      public:
        Duration(int64_t timestamp=1, Rational base=Rational());
        
        /*
        virtual ~Duration();
        Duration rescaleTo(Rational );
        Rational getTimeBase();
        int64_t getDuration();
        bool operator==(Duration t);
        */
      };
    }
  }
}

#endif	/* DURATION_H */

