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
#include "org/esb/hive/Environment.h"
using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
  org::esb::hive::Environment::build(argc, argv);
  Log::open();
  org::esb::core::PluginRegistry::getInstance()->load(HTTPPULLSOURCE_PLUGIN);
  org::esb::core::PluginRegistry::getInstance()->initPlugins();
  org::esb::core::PluginRegistry::getInstance()->startServerServices();
  std::map<std::string, std::string> cfg;
  //http://trac.codergrid.de/repository/test.mp4
  //cfg["data"]="{\"url\":\"http://mirrorblender.top-ix.org/peach/bigbuckbunny_movies/big_buck_bunny_480p_h264.mov\"}";
  if(argc>1){
    cfg["data"]=std::string("{\"url\":\"")+argv[1]+"\"}";
  }else{
    cfg["data"]="{\"url\":\"/media/video/big_buck_bunny_480p_surround-fix.avi\"}";
  }
  //cfg["downloadtask.trg"]="/tmp/index.html";
  try{
    Ptr<org::esb::core::Task> task = org::esb::core::PluginRegistry::getInstance()->createTask("HTTPPullSource", cfg);
    if (task) {
      task->prepare();
      task->execute();
    }else{
      LOGDEBUG("task not found");
    }
  }catch(const org::esb::core::TaskException & ex){
    LOGERROR(ex.what());
    //LOGERROR(ex.displayText());
  }
  org::esb::core::PluginRegistry::getInstance()->stopServices();
  org::esb::core::PluginRegistry::close();
  org::esb::config::Config::close();
  return 0;
}

