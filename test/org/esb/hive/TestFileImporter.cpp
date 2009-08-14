#include "org/esb/config/config.h"
#include "org/esb/hive/FileImporter.h"
using namespace org::esb::config;

int main() {
//   av_register_all();
  Config::setProperty("db.connection", "mysql:host=127.0.0.1;db=hive2;user=root;passwd=");
  char * argv[] = {"", (char*) "../../../../../test.dvd"};
//  logdebug("TestFileIMport");
  int fileid = import(2, argv);
}
