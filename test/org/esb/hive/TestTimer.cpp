/* 
 * File:   TestTimer.cpp
 * Author: jhoelscher
 *
 * Created on 10. Dezember 2009, 14:32
 */

#include <stdlib.h>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include "org/esb/util/Log.h"
#include "org/esb/lang/Thread.h"

class Timer {
public:

  Timer() : timer(io_timer) {
    timer.expires_from_now(boost::posix_time::seconds(5));
    timer.async_wait(boost::bind(&Timer::handle_timeout, this, boost::asio::error::operation_aborted));
    boost::thread th(boost::bind(&boost::asio::io_service::run, &io_timer));
  }

  void next() {
    timer.async_wait(boost::bind(&Timer::handle_timeout, this, boost::asio::placeholders::error));
  }
private:
//    t.expires_from_now(boost::posix_time::seconds(60));
//    t.async_wait(boost::bind(&DataHandler::remove_endpoint_from_stream, this, boost::asio::error::operation_aborted));

  void handle_timeout(const boost::system::error_code & er) {
    logdebug("timeout" << er);
    if (er == boost::asio::error::operation_aborted) {
      logdebug("Timer Event was Canceled");
      timer.expires_from_now(boost::posix_time::seconds(5));
      //timer.expires_at(timer.expires_at() + boost::posix_time::seconds(5));
      timer.async_wait(boost::bind(&Timer::handle_timeout, this, boost::asio::placeholders::error));
      return;
    }

    if(!er){
      timer.expires_from_now(boost::posix_time::seconds(5));
      //timer.expires_at(timer.expires_at() + boost::posix_time::seconds(5));
      timer.async_wait(boost::bind(&Timer::handle_timeout, this, boost::asio::placeholders::error));
    }
  }
  boost::asio::io_service io_timer;
  boost::asio::deadline_timer timer;

};

/*
 * 
 */
int main(int argc, char** argv) {
  Timer t;
/*
  for (int a = 0; a < 10; a++) {
    t.next();
    org::esb::lang::Thread::sleep2(4000);
  }*/
  org::esb::lang::Thread::sleep2(20000);
  return (EXIT_SUCCESS);
}
/*
#include <boost/asio.hpp>
#include <boost/noncopyable.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>

struct active_object : boost::noncopyable {

  active_object() :
  timer(scheduler, boost::posix_time::seconds(15)) {
    timer.async_wait(boost::bind(&active_object::timer_expired, this, boost::asio::placeholders::error));
    execution_thread.reset(new boost::thread(
        boost::bind(&boost::asio::io_service::run, &scheduler)));
  };

  virtual ~active_object() {
    scheduler.interrupt();
    execution_thread->join();
  };

  void some_operation() {
    scheduler.post(boost::bind(&active_object::some_operation_impl, this));
  };

protected:

  boost::asio::io_service scheduler;

private:

  boost::asio::deadline_timer timer;
  boost::shared_ptr<boost::thread> execution_thread;

  void some_operation_impl() {
    // do something here...
    std::cout << "Do something." << std::endl;
  };

  void timer_expired(boost::asio::error const & error) {
    timer.expires_at(timer.expires_at() + boost::posix_time::seconds(15));
    if (!error)
      timer.async_wait(boost::bind(&active_object::timer_expired,
        this,
        boost::asio::placeholders::error)
        );
  };

};
 * */