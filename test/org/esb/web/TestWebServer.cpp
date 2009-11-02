#include "org/esb/config/config.h"

#include "org/esb/web/WebServer.h"
#include "org/esb/lang/Thread.h"
#include <boost/thread.hpp>
using namespace org::esb::lang;
using namespace org::esb::web;
using namespace org::esb::config;

WebServer * w;


int main(int argc, char**argv) {
  //  Config::init("");
  Config::setProperty("db.connection", "mysql:host=127.0.0.1;db=hive2;user=root;passwd=");

  std::string base_path = MEC_SOURCE_DIR;
  Config::setProperty("hive.base_path", base_path.c_str());
  std::string docroot = base_path.append("/web");
  Config::setProperty("web.docroot", docroot.c_str());

  int timeout=0;
  if(argc>1)
    timeout=atoi(argv[1]);
  w= new WebServer();
  w->start();
  do{
    Thread::sleep2(timeout>0?timeout:1000);
  }while(timeout==0);
  w->stop();
  delete w;
  Config::close();
}
