#include "org/esb/db/hivedb.hpp"
#include "org/esb/hive/protocol/DataHandler.cpp"
#include "org/esb/hive/job/ProcessUnitController.h"
#include "org/esb/io/FileOutputStream.h"
#include "org/esb/io/FileInputStream.h"
#include "org/esb/io/StringOutputStream.h"
#include "org/esb/io/StringInputStream.h"
#include "org/esb/io/ObjectOutputStream.h"
#include "org/esb/io/ObjectInputStream.h"
#include "org/esb/hive/DatabaseService.h"
#include "org/esb/signal/Messenger.h"
#include "org/esb/lang/Thread.h"

#include "org/esb/io/File.h"
#include "org/esb/hive/FileImporter.h"
#include "org/esb/hive/CodecFactory.h"
#include "org/esb/hive/JobUtil.h"
#include "config.h"
#include "org/esb/config/config.h"
//#include "org/esb/util/Log.h"
using namespace org::esb::io;
using namespace org::esb::hive;
using namespace org::esb::config;
bool running = true;

void processUnitReader() {
  LOGDEBUG("starting void processUnitReader()");
  while (running) {
    Message msg;
    msg.setProperty("processunitcontroller", "GET_PROCESS_UNIT");
    Messenger::getInstance().sendRequest(msg);
    boost::shared_ptr<ProcessUnit>unit = msg.getPtrProperty("processunit");

    if (unit->_input_packets.size() == 0) {
      LOGDEBUG("unit->_input_packets.size() == 0")
      org::esb::lang::Thread::sleep2(500);
    } else {
      msg.setProperty("processunitcontroller", "PUT_PROCESS_UNIT");
      Messenger::getInstance().sendRequest(msg);
    }
  }
}

void processUnitAudioReader() {
  while (running) {
    Message msg;

    msg.setProperty("processunitcontroller", "GET_AUDIO_PROCESS_UNIT");
    Messenger::getInstance().sendRequest(msg);
    boost::shared_ptr<ProcessUnit>unit = msg.getPtrProperty("processunit");

    if (unit->_input_packets.size() == 0) {
      LOGDEBUG("unit->_input_packets.size() == 0")
      org::esb::lang::Thread::sleep2(500);
    } else {
      msg.setProperty("processunitcontroller", "PUT_PROCESS_UNIT");
      Messenger::getInstance().sendRequest(msg);
    }

  }
}

int main() {
  Log::open("");
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
  DatabaseService::start(MEC_SOURCE_DIR);



  if (!DatabaseService::databaseExist()) {
    DatabaseService::createDatabase();
  }
  //      DatabaseService::createDatabase();
  DatabaseService::dropTables();
  DatabaseService::updateTables();
  DatabaseService::loadPresets();



  std::string src = MEC_SOURCE_DIR;
  src.append("/test.dvd");
  {
  org::esb::hive::FileImporter imp;

  db::MediaFile mediafile = imp.import(org::esb::io::File(src));
  assert(mediafile.id > 0);
  //db::HiveDb db("mysql", org::esb::config::Config::getProperty("db.url"));
  db::Profile p=litesql::select<db::Profile>(mediafile.getDatabase(),db::Profile::Id==1).one();
  int jobid = org::esb::hive::JobUtil::createJob(mediafile, p, "/tmp");
  assert(jobid > 0);
  }
  {

    ProcessUnitController ctrl;
    Messenger::getInstance().addMessageListener(ctrl);
    Messenger::getInstance().sendMessage(Message().setProperty("processunitcontroller", org::esb::hive::START));
    org::esb::lang::Thread::sleep2(3000);

    std::string indata;
    std::string outdata;

    StringOutputStream fos(outdata);

    StringInputStream fis(indata);

    boost::asio::ip::tcp::endpoint e(boost::asio::ip::address_v4::from_string("127.0.0.1"), 6000);

    //	DataHandler handler(&fis, &fos, e);
    //	handler.process("get process_unit");
    org::esb::lang::Thread::sleep2(5000);


    boost::thread t1(processUnitReader);
    boost::thread t2(processUnitAudioReader);

    org::esb::lang::Thread::sleep2(10000);
    running = false;
    org::esb::lang::Thread::sleep2(1000);
    Messenger::getInstance().sendRequest(Message().setProperty("processunitcontroller", org::esb::hive::STOP));
    org::esb::lang::Thread::sleep2(10000);
    CodecFactory::free();

    //    Messenger::free();
  }
  DatabaseService::stop();
  Log::close();
  return 0;
}