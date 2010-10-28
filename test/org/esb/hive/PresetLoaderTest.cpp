/* 
 * File:   PresetLoaderTest.cpp
 * Author: HoelscJ
 *
 * Created on 26. Oktober 2010, 11:28
 */

#include "org/esb/hive/PresetLoader.h"
#include <cstdlib>
#include "org/esb/hive/PresetReader.h"
#include "org/esb/hive/DatabaseService.h"

#include "org/esb/util/Log.h"

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
  Log::open("");
  std::string path=MEC_SOURCE_DIR;
  org::esb::hive::PresetReader reader(path+"/res/presets/test.preset");
  LOGDEBUG(reader.toString());


  org::esb::hive::PresetLoader loader(reader);

  org::esb::hive::DatabaseService::start(MEC_SOURCE_DIR);
  if (org::esb::hive::DatabaseService::databaseExist()) {
    org::esb::hive::DatabaseService::dropDatabase();
  }

  org::esb::hive::DatabaseService::createDatabase();
  org::esb::hive::DatabaseService::createTables();
  loader.load();
  return 0;
}

