/* 
 * File:   ProcessUnitControllerTest.cpp
 * Author: HoelscJ
 *
 * Created on 19. Juli 2010, 16:39
 */

#include <cstdlib>
#include "org/esb/hive/job/ProcessUnitController.h"
#include "org/esb/hive/DatabaseService.h"
#include "org/esb/signal/Messenger.h"
#include "org/esb/hive/CodecFactory.h"
#include "org/esb/lang/Thread.h"
#include "org/esb/io/File.h"
#include "org/esb/hive/FileImporter.h"
#include "org/esb/hive/JobUtil.h"
#include "config.h"
#include "org/esb/config/config.h"
#include "org/esb/util/Log.h"

#include "boost/thread.hpp"
#include "boost/bind.hpp"
using namespace std;
using namespace org::esb::hive::job;
using namespace org::esb::hive;
using namespace org::esb::signal;
using namespace org::esb::config;

/*
 * 
 */

int running = true;

void processUnitReader() {
  LOGDEBUG("starting void processUnitReader()");
  while (running) {
    Message msg;
    msg.setProperty("processunitcontroller", "GET_PROCESS_UNIT");
    Messenger::getInstance().sendRequest(msg);
    boost::shared_ptr<ProcessUnit>unit = msg.getPtrProperty("processunit");

    if (unit->_input_packets.size() == 0) {
      org::esb::lang::Thread::sleep2(500);
    } else {
      msg.setProperty("processunitcontroller", "PUT_PROCESS_UNIT");
      Messenger::getInstance().sendRequest(msg);
    }

  }
}

void audioProcessUnitReader() {
  LOGDEBUG("starting void audioprocessUnitReader()");
  while (running) {
    Message msg;
    msg.setProperty("processunitcontroller", "GET_AUDIO_PROCESS_UNIT");
    Messenger::getInstance().sendRequest(msg);
    boost::shared_ptr<ProcessUnit>unit = msg.getPtrProperty("processunit");
    if (unit->_input_packets.size() == 0) {
      org::esb::lang::Thread::sleep2(500);
    } else {
      msg.setProperty("processunitcontroller", "PUT_PROCESS_UNIT");
      Messenger::getInstance().sendRequest(msg);
    }
  }
}

void testDirect() {

}

int main(int argc, char** argv) {
  Log::open("");
  std::string host = "host=";
  host += DEFAULT_DATABASE_HOST;
  host += ";user=root;port=3306;database=example";
  Config::setProperty("db.url", host.c_str());
  DatabaseService::start(MEC_SOURCE_DIR);
  {
    if (!DatabaseService::databaseExist()) {
      DatabaseService::createDatabase();
    }
    DatabaseService::dropTables();
    DatabaseService::updateTables();
    DatabaseService::loadPresets();



    std::string src = MEC_SOURCE_DIR;
    src.append("/test.dvd");

    int fileid = import(org::esb::io::File(src));
    assert(fileid > 0);
    int jobid = jobcreator(fileid, 1, "/tmp");
    assert(jobid > 0);

    ProcessUnitController ctrl;
    Messenger::getInstance().addMessageListener(ctrl);
    Messenger::getInstance().sendMessage(Message().setProperty("processunitcontroller", org::esb::hive::START));



    org::esb::lang::Thread::sleep2(5000);
    boost::thread t1(processUnitReader);
    boost::thread t2(audioProcessUnitReader);


    org::esb::lang::Thread::sleep2(100000);
    running = false;
    Messenger::getInstance().sendRequest(Message().setProperty("processunitcontroller", org::esb::hive::STOP));
    org::esb::lang::Thread::sleep2(1000);

    Messenger::free();

  }
  DatabaseService::stop();
  CodecFactory::free();
  Log::close();
  Config::close();

  return 0;
}

