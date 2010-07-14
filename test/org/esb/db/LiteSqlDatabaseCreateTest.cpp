/* 
 * File:   LiteSqlDatabaseCreateTest.cpp
 * Author: HoelscJ
 *
 * Created on 14. Juli 2010, 14:36
 */

#include <cstdlib>
//#include "litesql.hpp"
#include "org/esb/db/hivedb.hpp"
#include "mysql/mysql.h"
#include "org/esb/hive/DatabaseService.h"
#include "org/esb/util/Log.h"
using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
  Log::open("");
  org::esb::hive::DatabaseService::start("/home/HoelscJ/devel/mec");
  if (mysql_library_init(0, NULL, NULL))
    std::cout << "Error init mysql" << std::endl;
  db::HiveDb db("mysql", "host=127.0.0.1;user=root;port=3306;database=example");

  //  db.verbose = true;
  if (db.needsUpgrade()) {
    std::cout << "Upgrade database" << std::endl;
    db.upgrade();
  }

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
      std::cout<<(*it)<<std::endl;

    vector<db::Profile>::iterator itp = profiles.begin();
    for (; itp != profiles.end(); itp++){
      std::cout<<(*itp)<<std::endl;
    }
  }


  org::esb::hive::DatabaseService::stop();
  Log::close();
  return 0;
}

