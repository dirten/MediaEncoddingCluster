#include "Timer.h"
#include "org/esb/util/Log.h"
	Timer::~Timer(){
  	  logdebug("~Timer");
	  io_timer.stop();
	  _timer_thread->join();
	  logdebug("~Timer end");
	}
	void Timer::cancel(){
    timer.cancel();
  }
