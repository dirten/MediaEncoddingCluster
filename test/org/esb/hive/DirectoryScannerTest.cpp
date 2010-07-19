/* 
 * File:   DirectoryScannerTest.cpp
 * Author: HoelscJ
 *
 * Created on 16. Juli 2010, 15:27
 */

#include <cstdlib>
#include "org/esb/db/hivedb.hpp"
#include "org/esb/config/config.h"
#include "org/esb/hive/DatabaseService.h"
#include "org/esb/hive/DirectoryScanner.h"
#include "org/esb/signal/Messenger.h"
#include "org/esb/signal/Message.h"
#include "org/esb/lang/Thread.h"
#include "org/esb/util/Log.h"
using namespace std;
using namespace org::esb;

/*
 * 
 */
int main(int argc, char** argv) {
  Log::open("");
  config::Config::setProperty("db.url", "host=localhost;user=root;port=3306;database=example");

  hive::DatabaseService::start(MEC_SOURCE_DIR);
    hive::DatabaseService::createDatabase();
  hive::DatabaseService::dropTables();
  hive::DatabaseService::updateTables();
  hive::DatabaseService::loadPresets();

  {
    db::HiveDb db("mysql", org::esb::config::Config::getProperty("db.url"));
    db::Profile profile = litesql::select<db::Profile > (db, db::Profile::Id == 1).one();
    db::Watchfolder folder(db);
    folder.infolder = "/home/HoelscJ/old/media/video/";
    folder.outfolder = "/tmp/test";
    folder.update();
    folder.profile().link(profile);
    org::esb::hive::DirectoryScanner dirscan("", 1000);
    dirscan.scan();
  }
    hive::DatabaseService::dropDatabase();
  hive::DatabaseService::stop();
  Log::close();
  config::Config::close();
  return 0;
}

