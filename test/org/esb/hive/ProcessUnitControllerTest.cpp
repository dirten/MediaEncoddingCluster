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

//int running_audio = true;
int running_threads = 0;

void processUnitReader(std::string data, int wait, int b) {
  LOGDEBUG("starting void processUnitReader()");
  std::string get = "GET_";
  get += data;
  get += "PROCESS_UNIT";
  running_threads++;
  bool running = true;
  int try_count = 5;
  while (running && (b-- != 0)) {
    Message msg;
    msg.setProperty("processunitcontroller", get);
    Messenger::getInstance().sendRequest(msg);
    boost::shared_ptr<ProcessUnit>unit = msg.getPtrProperty("processunit");

    if ((!unit) || unit->_input_packets.size() == 0) {
      LOGDEBUG("unit->_input_packets.size() == 0");
      if (try_count-- == 0)
        running = false;
    } else {
      msg.setProperty("processunitcontroller", "PUT_PROCESS_UNIT");
      Messenger::getInstance().sendRequest(msg);
      try_count = 5;
    }
    org::esb::lang::Thread::sleep2(wait);
  }
  LOGDEBUG("exiting reader thread");
  running_threads--;
}

void testDirect() {

}

void testInterruptedEncoding() {
  //  DatabaseService::start(MEC_SOURCE_DIR);

  if (!DatabaseService::databaseExist()) {
    DatabaseService::createDatabase();
  }
  DatabaseService::dropTables();
  DatabaseService::updateTables();
  DatabaseService::loadPresets();
  {
  std::string src = MEC_SOURCE_DIR;
  src.append("/test.dvd");
  org::esb::hive::FileImporter imp;

  db::MediaFile mediafile = imp.import(org::esb::io::File(src));
  assert(mediafile.id > 0);
  db::Profile p = litesql::select<db::Profile > (mediafile.getDatabase(), db::Profile::Id == 1).one();

  int jobid = jobcreator(mediafile, p, "/tmp");
  assert(jobid > 0);
  db::HiveDb db("mysql", org::esb::config::Config::getProperty("db.url"));
  {
    db::Job job = litesql::select<db::Job > (db, db::Job::Id == jobid).one();
    vector<db::JobDetail>details = job.jobdetails().get().all();
    assert(details.size() == 2);
    vector<db::JobDetail>::iterator it = details.begin();
    for (; it != details.end(); it++) {
      LOGDEBUG((*it));
    }
  }
  ProcessUnitController ctrl;
  Messenger::getInstance().addMessageListener(ctrl);
  Messenger::getInstance().sendMessage(Message().setProperty("processunitcontroller", org::esb::hive::START));

  org::esb::lang::Thread::sleep2(3000);

  boost::thread t1(boost::bind(&processUnitReader, "", 100, 1));
  boost::thread t2(boost::bind(&processUnitReader, "AUDIO_", 500, 1));

  org::esb::lang::Thread::sleep2(1000);

  Messenger::getInstance().sendRequest(Message().setProperty("processunitcontroller", org::esb::hive::STOP));
  org::esb::lang::Thread::sleep2(500);

  {
    db::Job job = litesql::select<db::Job > (db, db::Job::Id == jobid).one();
    LOGDEBUG("" << job);
    vector<db::JobDetail>details = job.jobdetails().get().all();

    assert(details.size() == 2);

    assert(details[0].lastdts == 127800);
    assert(((int64_t) details[1].lastdts) == 1218160);
  }

  Messenger::getInstance().sendMessage(Message().setProperty("processunitcontroller", org::esb::hive::START));
  org::esb::lang::Thread::sleep2(3000);

  boost::thread(boost::bind(&processUnitReader, "", 100, 1));
  boost::thread(boost::bind(&processUnitReader, "AUDIO_", 500, 1));
  org::esb::lang::Thread::sleep2(1000);

  Messenger::getInstance().sendRequest(Message().setProperty("processunitcontroller", org::esb::hive::STOP));
  org::esb::lang::Thread::sleep2(500);




  Messenger::free();
  CodecFactory::free();
  }
}

void testFullFile() {
  if (!DatabaseService::databaseExist()) {
    DatabaseService::createDatabase();
  }
  DatabaseService::dropTables();
  DatabaseService::updateTables();
  DatabaseService::loadPresets();

  {

  std::string src = MEC_SOURCE_DIR;
  src.append("/test.dvd");
  org::esb::hive::FileImporter imp;

  db::MediaFile mediafile = imp.import(org::esb::io::File(src));
  assert(mediafile.id > 0);
  db::Profile p = litesql::select<db::Profile > (mediafile.getDatabase(), db::Profile::Id == 1).one();

  int jobid = jobcreator(mediafile, p, "/tmp");
  assert(jobid > 0);

  ProcessUnitController ctrl;
  Messenger::getInstance().addMessageListener(ctrl);
  Messenger::getInstance().sendMessage(Message().setProperty("processunitcontroller", org::esb::hive::START));



  org::esb::lang::Thread::sleep2(3000);
  boost::thread t1(boost::bind(&processUnitReader, "", 100, -1));
  boost::thread t2(boost::bind(&processUnitReader, "AUDIO_", 500, -1));

  org::esb::lang::Thread::sleep2(500);

  while (running_threads > 0)
    org::esb::lang::Thread::sleep2(1000);

  Messenger::getInstance().sendRequest(Message().setProperty("processunitcontroller", org::esb::hive::STOP));
  org::esb::lang::Thread::sleep2(100);

  Messenger::free();
  }
}

int main(int argc, char** argv) {
  Log::open("");
  std::string src = MEC_SOURCE_DIR;
  org::esb::config::Config::setProperty("hive.base_path", src.c_str());

  DatabaseService::start(MEC_SOURCE_DIR);
  std::string host = "host=";
  host += DEFAULT_DATABASE_HOST;
  host += ";user=root;port=3306;database=example";
  Config::setProperty("db.url", host.c_str());
  std::string tmp = MEC_SOURCE_DIR;
  tmp += "/tmp";
  org::esb::io::File f(tmp);
  if (!f.exists())
    f.mkdir();
  Config::setProperty("hive.base_path", MEC_SOURCE_DIR);
  testInterruptedEncoding();
  //  testFullFile();
  //  return 0;

  DatabaseService::stop();
  CodecFactory::free();
  Log::close();
  Config::close();

  return 0;
}

