/* 
 * File:   ExportTaskTest.cpp
 * Author: HoelscJ
 *
 * Created on 31. Oktober 2011, 14:25
 */

#include <cstdlib>
#include "org/esb/core/PluginRegistry.h"
#include "org/esb/core/Task.h"
#include "org/esb/config/config.h"

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
  Log::open();

    org::esb::core::PluginRegistry::getInstance()->load(EXPORTTASK_PLUGIN);
  org::esb::core::PluginRegistry::getInstance()->initPlugins();

  /*encoding is ready*/
  std::map<std::string, std::string> expcfg;
  //cfg["encodingtask.src"]=std::string(MEC_SOURCE_DIR).append("/test.dvd");
  expcfg["exporttask.trg"] = "/tmp/ChocolateFactory.mp4";
  expcfg["exporttask.jobid"] = "collector";
  expcfg["exporttask.format"] = "mp4";
  {
    Ptr<org::esb::core::Task> task = org::esb::core::PluginRegistry::getInstance()->createTask("ExportTask", expcfg);
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

