/* 
 * File:   QueueManagerTest.cpp
 * Author: HoelscJ
 *
 * Created on 17. September 2010, 13:06
 */

#include <cstdlib>
#include "org/esb/mq/QueueManager.h"
#include "org/esb/mq/QueueConnection.h"
#include "org/esb/lang/CtrlCHitWaiter.h"
#include "org/esb/lang/Thread.h"
#include "org/esb/io/File.h"
#include "org/esb/io/QueueOutputStream.h"
#include "org/esb/io/ObjectOutputStream.h"
#include "org/esb/io/QueueInputStream.h"
#include "org/esb/io/ObjectInputStream.h"
#include "org/esb/hive/job/ProcessUnit.h"
using namespace std;

using namespace org::esb::mq;
using namespace org::esb::lang;
using namespace org::esb::io;
using namespace org::esb::hive::job;

/*
 * 
 */
int main(int argc, char** argv) {
  File f("test.cfg");
  if(!f.exists())
    f.createNewFile();
  QueueManager man;
  man.start();
  {
  QueueConnection con("safmq://admin:@localhost:9000");
  if(!con.queueExist("testqueue"))
    con.createQueue("testqueue");
  }
  {
  QueueOutputStream q("safmq://admin:@localhost:9000/testqueue");
  ObjectOutputStream oos(&q);
  LOGDEBUG("writing 1000 ProcessUnits to the queue");
  ProcessUnit unit;
  for(int a=0;a<1000;a++)
    oos.writeObject(unit);
  }
  {
  QueueInputStream qin("safmq://admin:@localhost:9000/testqueue");
  ObjectInputStream ois(&qin);
  LOGDEBUG("reading 1000 ProcessUnits to the queue");
    ProcessUnit unit;
  for(int a=0;a<1000;a++)
    ois.readObject(unit);
  }
//  org::esb::lang::Thread::sleep2(10000);
//  CtrlCHitWaiter::wait();
  {
  QueueConnection con("safmq://admin:@localhost:9000");

  con.deleteQueue("testqueue");
  }
  man.stop();
  return 0;
}

