#include <cstdlib>
#include "org/esb/mq/QueueManager.h"
#include "org/esb/mq/QueueConnection.h"
#include "org/esb/lang/CtrlCHitWaiter.h"
#include "org/esb/lang/Thread.h"
using namespace std;

/*
* 
*/
int main(int argc, char** argv) {
  org::esb::mq::QueueManager man;
  man.start();
  org::esb::lang::Thread::sleep2(1000);
  {
    org::esb::mq::QueueConnection con("safmq://admin:@localhost:9000");
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