/* 
 * File:   QueueListenerTest.cpp
 * Author: HoelscJ
 *
 * Created on 17. Dezember 2010, 11:57
 */

#include <cstdlib>

#include "org/esb/mq/QueueConnection.h"
#include "org/esb/mq/QueueManager.h"
#include "org/esb/mq/QueueMessageListener.h"
#include "org/esb/mq/QueueMessage.h"
#include "org/esb/util/Log.h"
#include "org/esb/lang/Thread.h"
#include "org/esb/config/config.h"

using namespace std;
using namespace org::esb::mq;
using namespace org::esb::config;

class Listener: public QueueMessageListener{
public:
  void onMessage(QueueMessage & msg){
//    LOGDEBUG("Message arrived"<<msg.getMessageID());
  }
};
/*
 * 
 */
int main(int argc, char** argv) {
  Log::open("");
//  QueueManager man;
  //man.start();
  org::esb::lang::Thread::sleep2(1000);
//  LOGDEBUG("QueueConnection="<<man.getUrl());
  org::esb::mq::QueueConnection conl("localhost", 20200);

  org::esb::mq::QueueConnection conw("localhost", 20200);
  if(!conw.queueExist("testqueue"))
    conw.createQueue("testqueue");
  std::string label="testlable";

  Listener listener;
  conl.setMessageListener("testqueue",listener, "label=testlable");
  conl.startListener();
  LOGDEBUG("start")
  if(true){
    safmq::MessageQueue * queue=conw.getMessageQueue("testqueue");
    QueueMessage msg;

    for(int a=0;a<100000;a++){
      queue->Enqueue(msg);
    }
    delete queue;
  }
  if(true){
    safmq::MessageQueue * queue=conw.getMessageQueue("testqueue");
    QueueMessage msg;
    msg.setLabel(label);

    for(int a=0;a<100000;a++){
      queue->Enqueue(msg);
    }
    delete queue;
  }
  safmq::MessageQueue * queue=conw.getMessageQueue("testqueue");
  LOGDEBUG("giong to dequeue");
  while(false){
    QueueMessage msg;
    safmq::ErrorCode err=queue->Retrieve(true,5, msg);
    if(err==safmq::EC_TIMEDOUT)
      break;
  }
  delete queue;
  LOGDEBUG("delay 20 sec.")
  org::esb::lang::Thread::sleep2(10000);
  LOGDEBUG("enqueue again");
  if(true){
    safmq::MessageQueue * queue=conw.getMessageQueue("testqueue");
    QueueMessage msg;

    for(int a=0;a<100000;a++){
      queue->Enqueue(msg);
    }
    delete queue;
  }

  org::esb::lang::Thread::sleep2(120000);
  conl.stopListener();
  LOGDEBUG("stop")
  Config::close();
  Log::close();
  return 0;
}

