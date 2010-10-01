/* 
 * File:   MHiveMaster.cpp
 * Author: HoelscJ
 *
 * Created on 17. September 2010, 16:11
 */

#include <cstdlib>
#include "org/esb/hive/DatabaseService.h"
#include "config.h"
#include "org/esb/lang/Process.h"
#include "org/esb/lang/CtrlCHitWaiter.h"
#include "org/esb/lang/Thread.h"
#include "org/esb/util/Log.h"
#include "org/esb/io/File.h"
#include "org/esb/config/config.h"
#include "org/esb/mq/QueueConnection.h"
#include "org/esb/hive/HiveException.h"
#include "org/esb/hive/NodeResolver.h"

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
  Log::open("");
  /*initialise the config class*/
  org::esb::config::Config::init("");


  /*check if database data directory exist*/
  org::esb::io::File datadir(org::esb::config::Config::get("hive.data_path"));
  if (!datadir.exists()) {
    datadir.mkdir();
  }

  /*starting the bundled Database Server*/
  org::esb::hive::DatabaseService::start(org::esb::config::Config::get("mhive.base_path"));

  Log::open("");
  org::esb::io::File f=org::esb::io::File(argv[0]);
  /*
  std::string exe = f.getPath() + "/mhivequeue";
#ifdef WIN32
  exe.append(".exe");
#endif
  LOGDEBUG("test if the queue manager is running");
  bool isQueueRunning=false;
  try{
      org::esb::mq::QueueConnection("safmq://admin:@localhost:20200");
      isQueueRunning=true;
    }catch(org::esb::hive::HiveException & ex){
      LOGINFO("queue manager not running");
    }
  LOGDEBUG("starting mhivequeue process");
  org::esb::lang::Process pQueue(exe, std::list<std::string>(), "mhivequeue");
  pQueue.run(true);
  LOGDEBUG("waiting for the Queue Server to come up");
  bool waiting=true;
  while(waiting){
    try{
      org::esb::mq::QueueConnection("safmq://admin:@localhost:20200");
      waiting=false;
    }catch(org::esb::hive::HiveException & ex){
      LOGINFO("waiting for queue manager:" << ex.what());
      org::esb::lang::Thread::sleep2(1000);
    }
  }
  LOGDEBUG("the Queue Manager Server arrived!!!");
*/
  std::string exe = f.getPath() + "/mhivecore";
#ifdef WIN32
  exe.append(".exe");
#endif
  LOGDEBUG("starting mhivecore process");
  org::esb::lang::Process pCore(exe);
  pCore.run(true);

  exe = f.getPath() + "/mhivereader";
#ifdef WIN32
  exe.append(".exe");
#endif
  LOGDEBUG("starting mhivereader process");
  org::esb::lang::Process pReader(exe);
  pReader.run(true);

  exe = f.getPath() + "/mhivewriter";
#ifdef WIN32
  exe.append(".exe");
#endif
  LOGDEBUG("starting mhivewriter process");
  org::esb::lang::Process pWriter(exe);
  pWriter.run(true);

  LOGDEBUG("mhive started");
  org::esb::lang::CtrlCHitWaiter::wait();
  //pQueue.stop();
  pCore.stop();
  pReader.stop();
  pWriter.stop();
  org::esb::hive::DatabaseService::stop();
  return 0;
}

