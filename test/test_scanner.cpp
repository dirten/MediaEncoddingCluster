#include "org/esb/hive/DirectoryScanner.h"
#include "org/esb/lang/Thread.h"
#include "org/esb/config/config.h"
using namespace org::esb::hive;
using namespace org::esb::lang;
using namespace org::esb::config;
using namespace org::esb::signal;

int main(){
  Config::init("cluster.cfg");
  DirectoryScanner scan("/tmp/hivein", 120);
  scan.onMessage(Message().setProperty("directoryscan","start"));
  
  Thread::sleep(1000000000);
}