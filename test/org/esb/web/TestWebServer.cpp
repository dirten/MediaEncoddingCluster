#include "org/esb/config/config.h"

#include "org/esb/web/WebServer.h"
#include "org/esb/lang/Thread.h"
#include <boost/thread.hpp>
#include "org/esb/av/AV.h"

#include "org/esb/util/Log.h"
#include "org/esb/hive/DatabaseService.h"
using namespace org::esb::lang;
using namespace org::esb::web;
using namespace org::esb::config;

WebServer * w;


int main(int argc, char**argv) {
   av_register_all();
  std::string base_path = MEC_SOURCE_DIR;
  Config::setProperty("db.connection", "mysql:host=127.0.0.1;db=hive2;user=root;passwd=");
  Config::setProperty("hive.base_path", base_path.c_str());
  org::esb::hive::DatabaseService::start(base_path);
  std::string docroot = base_path.append("/web");
  Config::setProperty("web.docroot", docroot.c_str());
  Config::init("../../mec/.hive");

  int timeout=0;
  if(argc>1)
    timeout=atoi(argv[1]);
  w= new WebServer();
  w->run();
/*
  do{
    Thread::sleep2(timeout>0?timeout:1000);
  }while(timeout>0);
 */
  w->stop();
  delete w;
  org::esb::hive::DatabaseService::stop();
  Config::close();
  logdebug("close log");
  Log::close();
}
