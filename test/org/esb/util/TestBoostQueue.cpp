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
    try{
    mq = new message_queue
      (open_or_create //open or create
      , "message_queue" //name
      , 100 //max message number
      , sizeof(int) //max message size
      );
    LOGDEBUG("Max Message:"<<mq->get_max_msg());
    }catch(interprocess_exception &ex){
	LOGERROR(ex.what());
    }
  }

  void send() {
    for (int a = 0; a < 10; a++) {
      mq->send(&a, sizeof (a), 0);
    }
  }

  ~queue_sender() {
    LOGDEBUG("remove queue")
    if(!message_queue::remove("message_queue")){
	LOGERROR("removing queue")
    }
    LOGDEBUG("queue removed")
//      delete mq;
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
    LOGDEBUG("Max Messages:"<<mq->get_max_msg());
    LOGDEBUG("Num Messages"<<mq->get_num_msg());
  }

  void receive() {
    int number=0;
    unsigned int prio;
    std::size_t recvd_size;
    try {
      mq->receive(&number, sizeof (number), (std::size_t &)recvd_size, prio);
    } catch (interprocess_exception &ex) {
      LOGERROR(ex.what());
    }

    LOGDEBUG("Received number:" << number);
  }

  ~queue_receiver() {
    delete mq;
  }
private:
  message_queue * mq;

};

int main(int argc, char** argv) {
  Log::open("");
  //message_queue::remove("message_queue");
  if(argc==2&&strcmp(argv[1],"send")==0){
    queue_sender s;
    s.send();
    Thread::sleep2(100000);
  }
  if(argc==2&&strcmp(argv[1],"recv")==0){
    queue_receiver r;
    r.receive();
  }
  //queue_sender s;
  //boost::thread sender_th(boost::bind(&queue_sender::send, &s));
  return (EXIT_SUCCESS);
}

