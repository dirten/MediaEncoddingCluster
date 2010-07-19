#include "org/esb/db/hivedb.hpp"

#include "org/esb/sql/Connection.h"
#include "org/esb/io/File.h"
#include "org/esb/io/FileInputStream.h"
#include "org/esb/util/StringTokenizer.h"
#include "org/esb/util/StringUtil.h"
#include "org/esb/config/config.h"
#include "org/esb/hive/DatabaseService.h"
using namespace org::esb;

int main() {
  Log::open("");
  std::string src = MEC_SOURCE_DIR;
  

  config::Config::setProperty("db.url", "host=localhost;user=root;port=3306;database=example");
  hive::DatabaseService::start(src);

  
  hive::DatabaseService::createDatabase();
  hive::DatabaseService::createTables();
  hive::DatabaseService::dropTables();
  hive::DatabaseService::dropDatabase();


  hive::DatabaseService::stop();
  config::Config::close();
  Log::close();
}

