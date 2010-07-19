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
#include "org/esb/lang/Thread.h"
#include "org/esb/io/File.h"
#include "org/esb/hive/FileImporter.h"
#include "org/esb/hive/JobUtil.h"
#include "config.h"
#include "org/esb/config/config.h"
#include "org/esb/util/Log.h"
using namespace std;
using namespace org::esb::hive::job;
using namespace org::esb::hive;
using namespace org::esb::signal;
using namespace org::esb::config;

/*
 * 
 */

void testDirect(){

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

    ProcessUnitController ctrl;
    Messenger::getInstance().addMessageListener(ctrl);
    Messenger::getInstance().sendMessage(Message().setProperty("processunitcontroller", org::esb::hive::START));

    org::esb::lang::Thread::sleep2(5000);
    std::string src = MEC_SOURCE_DIR;
    src.append("/test.dvd");

    int fileid = import(org::esb::io::File(src));
    assert(fileid > 0);
    int jobid = jobcreator(fileid, 1, "/tmp");
    assert(jobid > 0);


    org::esb::lang::Thread::sleep2(10000);
    Messenger::getInstance().sendRequest(Message().setProperty("processunitcontroller", org::esb::hive::STOP));
    org::esb::lang::Thread::sleep2(10000);
    
    Messenger::free();

  }
  DatabaseService::stop();
  Log::close();
  Config::close();

  return 0;
}

