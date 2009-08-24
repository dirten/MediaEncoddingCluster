#include "org/esb/config/config.h"
#include "org/esb/hive/job/ClientHandler.h"
#include "org/esb/hive/job/ProcessUnit.h"

using namespace org::esb::hive::job;
int main() {
  org::esb::config::Config::setProperty("hive.base_path","/home/jhoelscher/mec");
  org::esb::config::Config::setProperty("db.connection","mysql:host=10.122.6.40;db=hive2;user=jhoelscher;passwd=erkentnis");

  ClientHandler * handler=new ClientHandler();

  ProcessUnit pu;
  handler->getProcessUnit(pu);
  
  delete handler;
  org::esb::config::Config::close();
  mysql_library_end();

  return 0;
}

