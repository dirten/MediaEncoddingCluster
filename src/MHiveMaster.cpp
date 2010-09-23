/* 
 * File:   MHiveMaster.cpp
 * Author: HoelscJ
 *
 * Created on 17. September 2010, 16:11
 */

#include <cstdlib>
#include "org/esb/hive/DatabaseService.h"
#include "org/esb/lang/Process.h"
#include "org/esb/lang/CtrlCHitWaiter.h"
#include "org/esb/lang/Thread.h"
#include "org/esb/util/Log.h"
#include "org/esb/io/File.h"
#include "org/esb/config/config.h"
#include "org/esb/mq/QueueConnection.h"
using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
  Log::open("");
  /*initialise the config class*/
  org::esb::config::Config::init("");


  /*check if database directory exist*/
  org::esb::io::File datadir(org::esb::config::Config::get("MYSQL_DATA"));
  if (!datadir.exists()) {
    datadir.mkdir();
    /**
     * need to create a mysql database bootstrap
     */
    org::esb::hive::DatabaseService::bootstrap();
  }
  /*starting the bundled Database Server*/
  org::esb::hive::DatabaseService::start(org::esb::config::Config::get("mhive.base_path"));

  Log::open("");
  org::esb::io::File f(".");
  std::string exe = f.getPath() + "/mhivequeue";
#ifdef WIN32
  exe.append(".exe");
#endif
  LOGDEBUG("starting mhivequeue process");
  org::esb::lang::Process pQueue(exe, std::list<std::string>(), "mhivequeue");
  pQueue.run(true);
  LOGDEBUG("waiting for the Queue Server to come up");
  bool waiting=true;
  while(waiting){
    try{
    org::esb::mq::QueueConnection("safmq://admin:@localhost:20200");
    waiting=false;
    }catch(tcpsocket::SocketException & ex){
      LOGERROR("waiting for queue manager:" << ex.what());
      org::esb::lang::Thread::sleep2(1000);
    }
  }

  exe = f.getPath() + "/mhivecore";
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
  pQueue.stop();
  pCore.stop();
  pReader.stop();
  pWriter.stop();
  org::esb::hive::DatabaseService::stop();
  return 0;
}

