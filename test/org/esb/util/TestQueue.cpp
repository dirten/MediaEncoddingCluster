#include "org/esb/util/Queue.h"

#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include "org/esb/util/Log.h"

#include "org/esb/lang/Thread.h"
using namespace org::esb::util;
using namespace org::esb::lang;

  Queue<int> q;

void queue_receiver(){
  for(int a=0;a<10;a++){
    /*will block on empty queue*/
    int c = q.dequeue();
    logdebug("dequeue:"<<c);
    Thread::sleep2(1000);
  }
}

void queue_sender(){
  for(int a=0;a<10;a++){
    logdebug("enqueue:"<<a);
    q.enqueue(a);
  }
}

int main(){
  queue_sender();
  Thread::sleep2(2000);
  boost::thread t(boost::bind(&queue_receiver));
  t.join();
}
