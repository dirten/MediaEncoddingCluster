#ifndef ORG_ESB_UTIL_SCOPEDTIMECOUNTER_H
#define ORG_ESB_UTIL_SCOPEDTIMECOUNTER_H
#include <string>
#include "boost/date_time/posix_time/posix_time.hpp"
//#include "Log.h"
#include "exports.h"
namespace org {
  namespace esb {
    namespace util {

      using namespace boost::posix_time;

      class UTIL_EXPORT ScopedTimeCounter {
        //classlogger("org.esb.util.ScopedTimeCounter")
      public:
        ScopedTimeCounter(std::string name);
        ~ScopedTimeCounter(void);
        int64_t getMilliSec();
      private:
        ptime _start;
        ptime _end;
        std::string _name;
      };
    }
  }
}
#endif

