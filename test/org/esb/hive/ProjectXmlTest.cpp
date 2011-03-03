/* 
 * File:   ProjectXmlTest.cpp
 * Author: HoelscJ
 *
 * Created on 28. Februar 2011, 13:12
 */

#include <cstdlib>
#include "org/esb/db/hivedb.hpp"
#include "org/esb/util/Log.h"
#include "org/esb/hive/xml/Project.h"
#include "org/esb/hive/DatabaseService.h"
#include "org/esb/io/File.h"
#include "org/esb/io/FileInputStream.h"

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
  Log::open();
  org::esb::hive::DatabaseService::createDatabase();
  org::esb::hive::DatabaseService::updateTables();
  db::HiveDb db = org::esb::hive::DatabaseService::getDatabase();
  db::Project prj(db);
  prj.update();
  org::esb::hive::Project project(prj);

  org::esb::io::File file(std::string(MEC_SOURCE_DIR) + "/test/testproject.xml");
  if (!file.exists()) {
    LOGERROR("Preset File does not exist! " << file.getPath());
    return -1;
  }
  //LOGINFO("reading presets from "<<filename);
  org::esb::io::FileInputStream fis(&file);
  std::string data;
  fis.read(data);
  project.setXml(data);

  Log::close();

  return 0;
}

