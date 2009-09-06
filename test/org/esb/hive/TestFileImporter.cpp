#include "org/esb/config/config.h"
#include "org/esb/hive/FileImporter.h"
#include "org/esb/config/config.h"
using namespace org::esb;

int main() {
//   av_register_all();
	config::Config::setProperty("db.connection", "mysql:host=127.0.0.1;db=hive2;user=root;passwd=");
  std::string src = MEC_SOURCE_DIR;
  config::Config::setProperty("hive.base_path", src.c_str());
  config::Config::setProperty("db.connection", "mysql:host=;db=hive;user=;passwd=");

  src.append("/test.dvd");
	char * argv[] = {"", (char*) src.c_str()};
//  logdebug("TestFileIMport");
  int fileid = import(2, argv);
}
