/* 
 * File:   GraphVerfierTest.cpp
 * Author: HoelscJ
 *
 * Created on 7. Dezember 2011, 13:26
 */

#include <cstdlib>
#include "org/esb/core/PluginRegistry.h"
#include "org/esb/hive/Environment.h"
#include "org/esb/util/Log.h"

#include "plugins/hooks/jsonapi/GraphVerifier.h"
#include "org/esb/io/File.h"
#include "org/esb/io/FileInputStream.h"

#include "org/esb/libjson/libjson.h"
using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
  Log::open();
  org::esb::core::PluginRegistry::getInstance()->load(ENCODINGTASK_PLUGIN);
  org::esb::core::PluginRegistry::getInstance()->load(DOWNLOADTASK_PLUGIN);
  org::esb::core::PluginRegistry::getInstance()->load(EXPORTTASK_PLUGIN);

  org::esb::hive::Environment::build(argc, argv);
  org::esb::io::File graph_file(std::string(MEC_SOURCE_DIR) + "/test-data/test.graph");
  if (graph_file.exists()) {
    std::string graphdata;
    org::esb::io::FileInputStream fis(&graph_file);
    fis.read(graphdata);
    if (libjson::is_valid(graphdata)) {
      LOGDEBUG("Data is valid");
      JSONNode inode = libjson::parse(graphdata);
      graph::GraphVerifier * v = graph::GraphVerifier::getInstance();
      if (!v->verifyTasks(inode)) {
        LOGERROR(v->getResult().write_formatted());
      }else
        LOGDEBUG("Graph is valid and executable");
    }
  }
  //Log::open();
  //org::esb::core::PluginRegistry::getInstance()->load(ENCODINGTASK_PLUGIN);
  //org::esb::core::PluginRegistry::getInstance()->initPlugins();
  return 0;
}

