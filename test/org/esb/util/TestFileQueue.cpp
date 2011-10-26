#include "org/esb/util/Queue.h"

#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include "org/esb/util/Log.h"

#include "org/esb/lang/Thread.h"
using namespace org::esb::util;
using namespace org::esb::lang;

  Queue<int> q;

void queue_receiver(int msgcount, int wait){
  for(int a=0;a<msgcount;a++){
    /*will block on empty queue*/
    int c = q.dequeue();
    LOGDEBUG("dequeue:"<<c);
    Thread::sleep2(wait);
  }
}

void queue_sender(int count,int wait){
  for(int a=0;a<count;a++){
    LOGDEBUG("enqueue:"<<a);
    q.enqueue(a);
    Thread::sleep2(wait);
  }
}

void test_single_sender2receiver(){
  boost::thread t(boost::bind(&queue_receiver, 10,1));
  Thread::sleep2(100);
  queue_sender(10,0);
  t.join();
}
void test_single_sender2receiver2(){
  boost::thread t(boost::bind(&queue_sender, 10,0));
  Thread::sleep2(100);
  queue_receiver(10,100);
  t.join();
}
void test_multi_sender2single_receiver(){
  int msgcount=100;
  boost::thread t(boost::bind(&queue_receiver, msgcount*3,10));
  Thread::sleep2(100);
  boost::thread ts1(boost::bind(&queue_sender,msgcount, 10));
  boost::thread ts2(boost::bind(&queue_sender,msgcount, 20));
  boost::thread ts3(boost::bind(&queue_sender,msgcount, 30));

  t.join();
  ts1.join();
  ts2.join();
  ts3.join();
}

void test_multi_receiver2single_sender(){
  int msgcount=100;
  boost::thread t(boost::bind(&queue_receiver, msgcount,10));
  boost::thread t1(boost::bind(&queue_receiver, msgcount,20));
  boost::thread t2(boost::bind(&queue_receiver, msgcount,30));
  Thread::sleep2(100);
  boost::thread ts1(boost::bind(&queue_sender,msgcount*3, 10));

  t.join();
  t1.join();
  t2.join();
  ts1.join();
}

void test_multi_receiver2multi_sender(){
  int msgcount=100;
  boost::thread t(boost::bind(&queue_receiver, msgcount*2,1));
  boost::thread t1(boost::bind(&queue_receiver, msgcount*2,1));
  boost::thread t2(boost::bind(&queue_receiver, msgcount*2,1));
  Thread::sleep2(2000);
  boost::thread ts1(boost::bind(&queue_sender,msgcount, 1));
  boost::thread ts4(boost::bind(&queue_sender,msgcount, 1));
  boost::thread ts5(boost::bind(&queue_sender,msgcount, 1));
  boost::thread ts6(boost::bind(&queue_sender,msgcount, 1));
  boost::thread ts2(boost::bind(&queue_sender,msgcount, 2));
  boost::thread ts3(boost::bind(&queue_sender,msgcount, 3));

  t.join();
  t1.join();
  t2.join();
  ts1.join();
  ts2.join();
  ts3.join();
  ts4.join();
  ts5.join();
  ts6.join();
}

int main(){
  Log::open("");
  test_single_sender2receiver();
  test_multi_sender2single_receiver();
  test_multi_receiver2single_sender();
  test_multi_receiver2multi_sender();
  //Log::close();
}
