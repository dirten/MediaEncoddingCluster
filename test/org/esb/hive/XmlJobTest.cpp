/* 
 * File:   XmlJobTest.cpp
 * Author: HoelscJ
 *
 * Created on 1. März 2011, 15:30
 */

#include <cstdlib>
#include "org/esb/db/hivedb.hpp"
#include "org/esb/util/Log.h"
#include "org/esb/hive/xml/JobCreator.h"
#include "org/esb/hive/DatabaseService.h"
#include "org/esb/io/File.h"
#include "org/esb/io/FileInputStream.h"
#include "org/esb/config/config.h"

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
  Log::open();
  org::esb::hive::DatabaseService::createDatabase();
  org::esb::hive::DatabaseService::updateTables();
  db::HiveDb db = org::esb::hive::DatabaseService::getDatabase();
  org::esb::config::Config::setProperty("preset.path",std::string(MEC_SOURCE_DIR)+"/presets");
  org::esb::io::File file(std::string(MEC_SOURCE_DIR) + "/test/testjob.xml");
  if (!file.exists()) {
    LOGERROR("Preset File does not exist! " << file.getPath());
    return -1;
  }
  //LOGINFO("reading presets from "<<filename);
  org::esb::io::FileInputStream fis(&file);
  std::string data;
  fis.read(data);
  org::esb::hive::JobCreator cr;
  cr.setXml(data);
  return 0;
}

