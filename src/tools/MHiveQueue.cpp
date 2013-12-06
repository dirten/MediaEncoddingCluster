#include <cstdlib>
#include "org/esb/mq/QueueManager.h"
#include "org/esb/mq/QueueConnection.h"
#include "org/esb/lang/CtrlCHitWaiter.h"
#include "org/esb/lang/Thread.h"
#include "org/esb/util/Log.h"
using namespace std;

/*
* 
*/
int main(int argc, char** argv) {

  ::Log::open("");
  org::esb::mq::QueueManager man;
  man.start();
  org::esb::lang::Thread::sleep2(500);
  {
//    LOGDEBUG("QueueUrl:"<<man.getUrl())
    org::esb::mq::QueueConnection con(man.getUrl());
    if(!con.queueExist("punitin"))
      con.createQueue("punitin");
    if(!con.queueExist("punitout"))
      con.createQueue("punitout");
    if(!con.queueExist("system"))
      con.createQueue("system");
  }

  org::esb::lang::CtrlCHitWaiter::wait();
  man.stop();

  return 0;
}