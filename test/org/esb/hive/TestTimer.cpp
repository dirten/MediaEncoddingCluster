/* 
 * File:   TestTimer.cpp
 * Author: jhoelscher
 *
 * Created on 10. Dezember 2009, 14:32
 */

#include <stdlib.h>
#include "org/esb/util/Timer.h"
#include "org/esb/util/Log.h"
#include "org/esb/lang/Thread.h"

  void handle_timeout(const boost::system::error_code & er) {
    logdebug("handle timeout" << er);
  }

/*
 * 
 */
int main(int argc, char** argv) {
	logdebug("start");
	boost::shared_ptr<Timer> ti(new Timer(3,&handle_timeout));
	org::esb::lang::Thread::sleep2(12000);
  logdebug("resetting");
	ti.reset();
	ti.reset();
	org::esb::lang::Thread::sleep2(12000);
//  t.cancel();
//  t.next();
//  org::esb::lang::Thread::sleep2(6000);
//  t.cancel();
//  org::esb::lang::Thread::sleep2(6000);

/*
  for (int a = 0; a < 10; a++) {
    t.next();
    org::esb::lang::Thread::sleep2(4000);
  }*/
//  org::esb::lang::Thread::sleep2(40000);
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

