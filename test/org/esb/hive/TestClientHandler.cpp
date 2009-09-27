#include "org/esb/config/config.h"
#include "org/esb/hive/job/ClientHandler.h"
#include "org/esb/hive/job/ProcessUnit.h"

using namespace org::esb::hive::job;
int main() {
	std::string src = MEC_SOURCE_DIR;
	org::esb::config::Config::setProperty("hive.base_path",src.c_str());
  org::esb::config::Config::setProperty("db.connection","mysql:host=;db=hive;user=;passwd=");

  ClientHandler * handler=new ClientHandler();

  boost::shared_ptr<ProcessUnit> pu=handler->getProcessUnit();
  
  delete handler;
  org::esb::config::Config::close();
  mysql_library_end();

  return 0;
}

