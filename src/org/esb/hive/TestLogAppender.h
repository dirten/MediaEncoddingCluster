/* 
 * File:   TestLogAppender.h
 * Author: HoelscJ
 *
 * Created on 7. Oktober 2011, 15:06
 */

#ifndef TESTLOGAPPENDER_H
#define	TESTLOGAPPENDER_H
#include "org/esb/util/Log.h"
namespace org {
  namespace esb {
    namespace hive {

      class TestLogAppender: public log4cplus::Appender {
      public:
        TestLogAppender(const log4cplus::helpers::Properties properties);
        virtual ~TestLogAppender();
      private:

      };
    }
  }
}

#endif	/* TESTLOGAPPENDER_H */

