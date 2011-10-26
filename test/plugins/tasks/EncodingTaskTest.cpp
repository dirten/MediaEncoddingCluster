/* 
 * File:   DownloadTaskTest.cpp
 * Author: HoelscJ
 *
 * Created on 19. Oktober 2011, 14:03
 */

#include <cstdlib>
#include "org/esb/core/PluginRegistry.h"
#include "org/esb/core/Task.h"

#include "org/esb/util/Log.h"
#include "org/esb/io/FileInputStream.h"
#include "org/esb/config/config.h"
using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
  Log::open();
  
  
  /*Loading profile from disk*/
  org::esb::io::FileInputStream fis(std::string(MEC_SOURCE_DIR).append("/presets/x264-hq.preset"));
  std::string profile_data;
  fis.read(profile_data);
  org::esb::core::PluginRegistry::getInstance()->load(ENCODINGTASK_PLUGIN);
  org::esb::core::PluginRegistry::getInstance()->initPlugins();
  std::map<std::string, std::string> cfg;
  cfg["encodingtask.src"]=std::string(MEC_SOURCE_DIR).append("/test.dvd");
  cfg["encodingtask.profile"]=profile_data;
  {
    Ptr<org::esb::core::Task> task = org::esb::core::PluginRegistry::getInstance()->createTask("EncodingTask", cfg);
    if (task) {
      task->prepare();
      task->execute();
      task->cleanup();
    }
  }
  org::esb::core::PluginRegistry::close();
  org::esb::config::Config::close();
  return 0;
}

