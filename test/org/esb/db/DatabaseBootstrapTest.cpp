/* 
 * File:   DatabaseBootstrapTest.cpp
 * Author: HoelscJ
 *
 * Created on 20. September 2010, 17:45
 */

#include <cstdlib>
#include "org/esb/hive/DatabaseService.h"
#include "org/esb/util/Log.h"
#include "org/esb/config/config.h"
#include "org/esb/io/File.h"
using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
  Log::open("");
  std::string src = MEC_SOURCE_DIR;
  org::esb::config::Config::setProperty("hive.base_path", src.c_str());

//  org::esb::io::File mysqldatadir(org::esb::config::Config::get("MYSQL_DATA"));
//  if(mysqldatadir.exists())
//    mysqldatadir.deleteFile();
  /*initialise the config class*/
  
  org::esb::hive::DatabaseService::bootstrap();
  return 0;
}

