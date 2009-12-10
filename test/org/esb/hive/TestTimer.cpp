/* 
 * File:   TestTimer.cpp
 * Author: jhoelscher
 *
 * Created on 10. Dezember 2009, 14:32
 */

#include <stdlib.h>
#include "org/esb/util/Log.h"
#include "org/esb/lang/Thread.h"
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
class Timer {
public:
  Timer():timer(io_timer,boost::posix_time::seconds(2)){
    timer.async_wait(boost::bind(&Timer::handle_timeout, this, boost::asio::error::timed_out));
    boost::thread th(boost::bind(&boost::asio::io_service::run, &io_timer));
  }

private:
  void handle_timeout(const boost::system::error_code & er){
    logdebug("timeout"<<er);
    if (er == boost::asio::error::timed_out)
      timer.async_wait(boost::bind(&Timer::handle_timeout, this, boost::asio::error::timed_out));

  }
  boost::asio::io_service io_timer;
  boost::asio::deadline_timer timer;

};

/*
 * 
 */
int main(int argc, char** argv) {
  Timer t;
  org::esb::lang::Thread::sleep2(20000);
  return (EXIT_SUCCESS);
}

