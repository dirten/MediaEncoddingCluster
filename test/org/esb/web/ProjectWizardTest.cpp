/* 
 * File:   PreojectWizardTest.cpp
 * Author: HoelscJ
 *
 * Created on 8. November 2010, 11:06
 */

#include "org/esb/hive/DatabaseService.h"
#include "config.h"
#include "org/esb/config/config.h"
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
  org::esb::hive::DatabaseService::start(MEC_SOURCE_DIR);
  if (!DatabaseService::databaseExist()) {
    DatabaseService::createDatabase();
  }
  DatabaseService::dropTables();
  DatabaseService::updateTables();
  Wt::Test::WTestEnvironment env;
  org::esb::web::WebApp2 app(env);
  {
    org::esb::web::v2::ProjectWizard wizard;

    wizard.open();
    wizard.save();
  }
  app.createProject();
  org::esb::config::Config::close();
  return 0;
}

