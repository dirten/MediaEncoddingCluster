/* 
 * File:   PluginLoaderTest.cpp
 * Author: HoelscJ
 *
 * Created on 30. August 2011, 13:15
 */

#include <cstdlib>
#include "org/esb/util/Log.h"
#include "org/esb/core/PluginLoader.h"
#include "org/esb/io/File.h"
using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
  Log::open();
  
  LOGDEBUG("test")
  org::esb::core::PluginLoader loader(WEBSERVER_PLUGIN);
  //org::esb::core::PluginLoader(JSONSERVICE_PLUGIN);
  return 0;
}

