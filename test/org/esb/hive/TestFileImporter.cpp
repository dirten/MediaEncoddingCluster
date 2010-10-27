#include "org/esb/db/hivedb.hpp"
#include "org/esb/config/config.h"
#include "org/esb/hive/FileImporter.h"
#include "org/esb/config/config.h"
#include "org/esb/hive/DatabaseService.h"
#include "org/esb/io/File.h"

//#include <assert.h>
using namespace org::esb;

int main() {
  Log::open("");
  std::string src = MEC_SOURCE_DIR;

  config::Config::setProperty("hive.base_path", src.c_str());
  config::Config::setProperty("db.url", "host=localhost;user=root;port=3306;database=example");
//  org::esb::config::Config::init("");
  src.append("/test.dvd");
  LOGDEBUG("opening database");

  hive::DatabaseService::start(MEC_SOURCE_DIR);
//  return 0;
  
  if (!hive::DatabaseService::databaseExist()) {
    hive::DatabaseService::createDatabase();
  }
  hive::DatabaseService::dropTables();
  hive::DatabaseService::updateTables();
  hive::DatabaseService::loadPresets();
  
  {
    org::esb::hive::FileImporter imp;
    db::MediaFile mediafile = imp.import(org::esb::io::File(src));
    assert(mediafile.id > 0);
    {
      //  db::HiveDb db("mysql", config::Config::getProperty("db.url"));

      LOGDEBUG((long long int) mediafile.filesize)
    }
  }
  //hive::DatabaseService::dropDatabase();
  hive::DatabaseService::stop();

  Log::close();
  config::Config::close();
  return 0;
}
