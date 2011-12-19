/* 
 * File:   DownloadTaskTest.cpp
 * Author: HoelscJ
 *
 * Created on 19. Oktober 2011, 14:03
 */

#include <cstdlib>
#include "org/esb/core/PluginRegistry.h"
#include "org/esb/core/Task.h"
#include "org/esb/core/TaskException.h"

#include "org/esb/util/Log.h"
#include "org/esb/config/config.h"
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
  try{
    Ptr<org::esb::core::Task> task = org::esb::core::PluginRegistry::getInstance()->createTask("InputTask", cfg);
    if (task) {
      task->prepare();
      task->execute();
    }else{
      LOGDEBUG("task not found");
    }
  }catch(const org::esb::core::TaskException & ex){
    LOGERROR(ex.displayText());
  }
  org::esb::core::PluginRegistry::close();
  org::esb::config::Config::close();
  return 0;
}

