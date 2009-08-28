#include "org/esb/config/config.h"

#include "org/esb/web/WebServer.h"
using namespace org::esb::web;
using namespace org::esb::config;

int main() {
  //  Config::init("");
  Config::setProperty("db.connection", "mysql:host=127.0.0.1;db=hive2;user=root;passwd=");

  std::string base_path = MEC_SOURCE_DIR;
  Config::setProperty("hive.base_path", base_path.c_str());
  std::string docroot = base_path.append("/web");
  Config::setProperty("web.docroot", docroot.c_str());


  WebServer * ws = new WebServer();
  ws->start();
  delete ws;
  Config::close();
}
