#include <boost/asio/deadline_timer.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include "exports.h"
#ifndef ORG_ESB_UTIL_TIMER_H
#define ORG_ESB_UTIL_TIMER_H
class UTIL_EXPORT Timer {
public:

    template<typename Handler>
    Timer(int sec, Handler func) : timer(io_timer) {
        _seconds = sec;
        timer.expires_from_now(boost::posix_time::seconds(_seconds));
        timer.async_wait(func);
        _timer_thread.reset(new boost::thread(boost::bind(&boost::asio::io_service::run, &io_timer)));
    }
    ~Timer();
    void cancel();
private:
    int _seconds;
    boost::shared_ptr<boost::thread> _timer_thread;
    boost::asio::io_service io_timer;
    boost::asio::deadline_timer timer;
};
#endif