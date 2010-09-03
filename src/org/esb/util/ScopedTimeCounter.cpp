#include "ScopedTimeCounter.h"
#include "Log.h"

ScopedTimeCounter::ScopedTimeCounter(std::string name) {
  _name = name;
  _start = microsec_clock::local_time();
}

ScopedTimeCounter::~ScopedTimeCounter(void) {
  _end = microsec_clock::local_time();
  LOGDEBUG(_name << " duration: " << to_simple_string(_end - _start));
  std::cout<<_name << " duration: " << to_simple_string(_end - _start)<<std::endl;
}
