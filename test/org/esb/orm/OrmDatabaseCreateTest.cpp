/* 
 * File:   LiteSqlDatabaseCreateTest.cpp
 * Author: HoelscJ
 *
 * Created on 14. Juli 2010, 14:36
 */

#include <cstdlib>
//#include "litesql.hpp"
//#include "org/esb/orm/hivedb.hpp"
#include "org/esb/hive/DatabaseService.h"
#include "org/esb/util/Log.h"
#include "org/esb/config/config.h"
using namespace std;
using namespace orm;
/*
 * 
 */
int main(int argc, char** argv) {
  Log::open("");
  org::esb::config::Config::setProperty("db.url", "host=localhost;user=root;port=3306;database=example");

  org::esb::hive::DatabaseService::start(MEC_SOURCE_DIR);
  if(org::esb::hive::DatabaseService::databaseExist()){
    org::esb::hive::DatabaseService::dropDatabase();
  }
  org::esb::hive::DatabaseService::createDatabase();
  org::esb::hive::DatabaseService::createTables();
  {

    HiveDb db=org::esb::hive::DatabaseService::getDatabase();
//    db.create();
    db.upgrade();
    MediaFile file(db);
    file.filename = "test";
    file.update();
    file.update();

    Project project(db);
    project.update();
    project.inmediafiles().link(file);

  }
//  org::esb::hive::DatabaseService::dropDatabase();
  org::esb::hive::DatabaseService::stop();
  Log::close();
  return 0;
}

