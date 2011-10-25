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
using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
  Log::open();
  org::esb::core::PluginRegistry::getInstance()->load(DOWNLOADTASK_PLUGIN);
  org::esb::core::PluginRegistry::getInstance()->initPlugins();
  std::map<std::string, std::string> cfg;
  cfg["downloadtask.src"]="http://www.google.de";
  cfg["downloadtask.trg"]="/tmp/index.html";
  {
    Ptr<org::esb::core::Task> task = org::esb::core::PluginRegistry::getInstance()->createTask("DownloadTask", cfg);
    if (task) {
      task->prepare();
      task->execute();
    }
  }
  org::esb::core::PluginRegistry::close();
  return 0;
}

