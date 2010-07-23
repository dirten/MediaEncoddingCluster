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
#include "org/esb/hive/JobUtil.h"
#include "config.h"
#include "org/esb/config/config.h"
//#include "org/esb/util/Log.h"
using namespace org::esb::io;
using namespace org::esb::hive;
using namespace org::esb::config;
int main(){
  Log::open("");
  std::string host = "host=";
  host += DEFAULT_DATABASE_HOST;
  host += ";user=root;port=3306;database=example";
  Config::setProperty("db.url", host.c_str());
  std::string tmp=MEC_SOURCE_DIR;
  tmp+="/tmp";
  org::esb::io::File f(tmp);
  if(!f.exists())
	  f.mkdir();
  Config::setProperty("hive.base_path", MEC_SOURCE_DIR);
  DatabaseService::start(MEC_SOURCE_DIR);

    
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
	
	org::esb::lang::Thread::sleep2(4000);
		std::string indata;
		std::string outdata;

	StringOutputStream fos(outdata);
	ObjectOutputStream oos(&fos);
	StringInputStream fis(indata);
	ObjectInputStream ois(&fis);
	boost::asio::ip::tcp::endpoint e(boost::asio::ip::address_v4::from_string("127.0.0.1"),6000);
	
	DataHandler handler(&fis, &fos, e);
	handler.process("get process_unit");
	Log::close();
	return 0;
}