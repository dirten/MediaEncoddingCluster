/* 
 * File:   SimpleEnqueue.cpp
 * Author: HoelscJ
 *
 * Created on 16. September 2010, 13:05
 */

#include <cstdlib>
#include "MQFactory.h"
#include "MessageQueue.h"
#include "org/esb/util/Log.h"
using namespace std;
using namespace safmq;
/*
 * 
 */
int main(int argc, char** argv) {
  Log::open("");
  MessageQueue * q;
  try{
    q=MQFactory::BuildQueueConnection("safmq://admin:@localhost:9000/testqueue","","");
  }catch(tcpsocket::SocketException & ex){
    LOGERROR(ex.what());
    LOGERROR(ex.getError());
  }
  QueueMessage msg;
  msg.setLabel("test message");
  char * data=new char[10000000];
  msg.getBufferStream()->write(data,10000000);
  //ErrorCode ec;
  for(int a=0;a<1000;a++)
    q->Enqueue(msg);
  //LOGDEBUG(EC_Decode(ec));
  
  return 0;
}

