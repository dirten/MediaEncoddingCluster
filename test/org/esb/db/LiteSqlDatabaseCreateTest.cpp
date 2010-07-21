/* 
 * File:   LiteSqlDatabaseCreateTest.cpp
 * Author: HoelscJ
 *
 * Created on 14. Juli 2010, 14:36
 */

#include <cstdlib>
//#include "litesql.hpp"
#include "org/esb/db/hivedb.hpp"
#include "org/esb/hive/DatabaseService.h"
#include "org/esb/util/Log.h"
#include "org/esb/config/config.h"
using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
  Log::open("");
  org::esb::config::Config::setProperty("db.url", "host=localhost;user=root;port=3306;database=example");

  org::esb::hive::DatabaseService::start(MEC_SOURCE_DIR);

  org::esb::hive::DatabaseService::createDatabase();
  org::esb::hive::DatabaseService::createTables();
  {

    db::HiveDb db("mysql", org::esb::config::Config::getProperty("db.url"));

    db::MediaFile file(db);
    file.filename = "test";
    file.update();
    file.update();

    db::Project project(db);
    project.update();
    project.mediafiles().link(file);



    db::Profile profile1(db);
    db::Profile profile2(db);

    profile1.update();
    profile2.update();

    project.profiles().link(profile1);
    project.profiles().link(profile2);


    vector<db::Project> cursor = litesql::select<db::Project > (db).all();
    vector<db::Project>::iterator it = cursor.begin();
    for (; it != cursor.end(); it++) {
      vector<db::Profile> profiles = (*it).profiles().get().all();
      std::cout << (*it) << std::endl;

      vector<db::Profile>::iterator itp = profiles.begin();
      for (; itp != profiles.end(); itp++) {
        std::cout << (*itp) << std::endl;
      }
    }
  }
  org::esb::hive::DatabaseService::dropDatabase();
  org::esb::hive::DatabaseService::stop();
  Log::close();
  return 0;
}

