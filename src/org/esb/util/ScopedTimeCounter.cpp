#include "ScopedTimeCounter.h"
//#include "Log.h"
namespace org {
  namespace esb {
    namespace util {

      ScopedTimeCounter::ScopedTimeCounter(std::string name) {
        _name = name;
        _start = microsec_clock::local_time();
      }

      ScopedTimeCounter::~ScopedTimeCounter(void) {
        _end = microsec_clock::local_time();
        //LOGDEBUG(_name << " duration: " << to_simple_string(_end - _start));
        //std::cout << _name << " duration: " << to_simple_string(_end - _start) << std::endl;
      }

      int64_t ScopedTimeCounter::getMilliSec(){
        _end = microsec_clock::local_time();
        time_duration td=_end - _start;
        return td.total_milliseconds();
      }
    }
  }
}
