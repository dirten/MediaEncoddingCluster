/*
 * File:   QueueManagerTest.cpp
 * Author: HoelscJ
 *
 * Created on 17. September 2010, 13:06
 */

#include <cstdlib>
#include "org/esb/mq/QueueManager.h"
#include "org/esb/mq/QueueConnection.h"
#include "org/esb/mq/ObjectMessage.h"
#include "org/esb/lang/CtrlCHitWaiter.h"
#include "org/esb/lang/Thread.h"
#include "org/esb/io/File.h"
//#include "org/esb/io/QueueOutputStream.h"
//#include "org/esb/io/ObjectOutputStream.h"
//#include "org/esb/io/QueueInputStream.h"
//#include "org/esb/io/ObjectInputStream.h"
#include "org/esb/hive/job/ProcessUnit.h"
#include "org/esb/hive/Environment.h"
#include "org/esb/config/config.h"
using namespace std;

using namespace org::esb::mq;
using namespace org::esb::lang;
using namespace org::esb::io;
using namespace org::esb::hive::job;

/*
 * 
 */
int main(int argc, char** argv) {
  org::esb::hive::Environment::build(argc,argv);
  org::esb::io::File d("data");
  if(!d.exists())
    d.mkdir();

  File f("test.cfg");
  if(!f.exists())
    f.createNewFile();
  QueueManager man(org::esb::config::Config::get("hive.data_path"));
  man.start();
  org::esb::lang::Thread::sleep2(1000);
  {
  QueueConnection con(man.getUrl());
  if(!con.queueExist("testqueue"))
    con.createQueue("testqueue");
  }
  ObjectMessage msg;
  ProcessUnit unit;
  msg.setObject(unit);
  msg.getObject(unit);

  /*
  {
  QueueOutputStream q(man.getUrl()+"/testqueue");
  ObjectOutputStream oos(&q);
  LOGDEBUG("writing 1000 ProcessUnits to the queue");
  ProcessUnit unit;
  for(int a=0;a<1000;a++)
    oos.writeObject(unit);
  }
  {
  QueueInputStream qin(man.getUrl()+"/testqueue");
  ObjectInputStream ois(&qin);
  LOGDEBUG("reading 1000 ProcessUnits to the queue");
    ProcessUnit unit;
  for(int a=0;a<1000;a++)
    ois.readObject(unit);
  }*/
//  org::esb::lang::Thread::sleep2(10000);
//  CtrlCHitWaiter::wait();
  {
  QueueConnection con(man.getUrl());

  con.deleteQueue("testqueue");
  }
  man.stop();

  return 0;
}

