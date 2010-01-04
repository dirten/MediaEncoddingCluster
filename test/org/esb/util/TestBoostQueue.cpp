/* 
 * File:   TestBoostQueue.cpp
 * Author: jhoelscher
 *
 * Created on 17. Dezember 2009, 11:52
 */

#include <stdlib.h>
#include <boost/interprocess/ipc/message_queue.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include "org/esb/lang/Thread.h"
#include "org/esb/util/Log.h"
/*
 * 
 */

using namespace boost::interprocess;
using namespace org::esb::lang;

class queue_sender {
public:

  queue_sender() {
    mq = new message_queue
      (open_or_create //open or create
      , "message_queue" //name
      , 100 //max message number
      , 100 //max message size
      );
    logdebug(mq);
  }

  void send() {
    for (int a = 4; a < 10; a++) {
  //    mq->send(&a, sizeof (a), 0);
    }
  }

  ~queue_sender() {
    logdebug("remove queue")
    message_queue::remove("message_queue");
    logdebug("queue removed")
      delete mq;
  }

private:
  message_queue * mq;
};

class queue_receiver {
public:

  queue_receiver() {
    mq = new message_queue
      (open_only //only open
      , "message_queue" //name
      );
  }

  void receive() {
    int number;
    unsigned int prio;
    std::size_t recvd_size;
    try {
      mq->receive(&number, sizeof (number), recvd_size, prio);
    } catch (interprocess_exception &ex) {
      logerror(ex.get_native_error());
    }

    logdebug("Received number:" << number);
  }

  ~queue_receiver() {
    delete mq;
  }
private:
  message_queue * mq;

};

int main(int argc, char** argv) {
  message_queue::remove("message_queue");
  queue_sender s;
  boost::thread sender_th(boost::bind(&queue_sender::send, &s));
  Thread::sleep2(1000);
  queue_receiver r;
  r.receive();
  r.receive();
  r.receive();
  r.receive();
  Thread::sleep2(10000);
  return (EXIT_SUCCESS);
}

