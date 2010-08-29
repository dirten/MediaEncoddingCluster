/* 
 * File:   DataSourceTest.cpp
 * Author: jholscher
 *
 * Created on 29. August 2010, 15:38
 */

#include <cstdlib>
#include "org/esb/av/MediaLocator.h"
#include "org/esb/av/DataSource.h"
#include "org/esb/util/Log.h"
using namespace std;
using namespace org::esb::av;

/*
 * 
 */
int main(int argc, char** argv) {
  Log::open("");
  std::string basepath = MEC_SOURCE_DIR;
  std::string path="file://";
  MediaLocator locator(path+basepath+"/test.dvd");
  DataSource source(locator);
  source.connect();
  Log::close();
  return 0;
}

