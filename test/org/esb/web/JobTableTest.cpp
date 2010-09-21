/* 
 * File:   JobTableTest.cpp
 * Author: HoelscJ
 *
 * Created on 13. August 2010, 17:40
 */

#include <cstdlib>
#include "config.h"
#include "org/esb/hive/DatabaseService.h"
#include "org/esb/av/AV.h"
#include "org/esb/config/config.h"
#include "org/esb/web/JobTable.h"
#include "org/esb/web/WebApp2.h"
#include "org/esb/lang/CtrlCHitWaiter.h"

#include "Wt/Test/WTestEnvironment"
using namespace std;
using namespace org::esb::config;
using namespace org::esb::hive;
/*
 * 
 */
int main(int argc, char** argv) {


  Log::open("");
  av_register_all();
  std::string base_path = MEC_SOURCE_DIR;

  std::string host = "host=";
  host += DEFAULT_DATABASE_HOST;
  host += ";user=root;port=3306;database=example";
  Config::setProperty("db.url", host.c_str());

  Config::setProperty("hive.base_path", base_path.c_str());
  org::esb::hive::DatabaseService::start(MEC_SOURCE_DIR);

  if (!DatabaseService::databaseExist()) {
    DatabaseService::createDatabase();
  }
  DatabaseService::dropTables();
  DatabaseService::updateTables();
  DatabaseService::loadPresets();
  {
  Wt::Test::WTestEnvironment env;
  org::esb::web::WebApp2 app(env);


  org::esb::web::JobTable* table = new org::esb::web::JobTable();
  table->refresh();
  if (argc > 1)
    org::esb::lang::CtrlCHitWaiter::wait();
  else
    Thread::sleep2(10000);
  delete table;
  }
org::esb::hive::DatabaseService::stop();
Config::close();
Log::close();
  //delete table;
  return 0;
}

