/* 
 * File:   GraphVerfierTest.cpp
 * Author: HoelscJ
 *
 * Created on 7. Dezember 2011, 13:26
 */

#include "plugins/hooks/jsonapi/GraphVerifier.h"
#include <cstdlib>
#include "org/esb/core/PluginRegistry.h"
#include "org/esb/hive/Environment.h"
#include "org/esb/util/Log.h"
#include "org/esb/config/config.h"

#include "org/esb/io/File.h"
#include "org/esb/io/FileInputStream.h"

#include "org/esb/libjson/libjson.h"
using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
  org::esb::hive::Environment::build(argc, argv);
  Log::open();
  org::esb::core::PluginRegistry::getInstance()->load(ENCODINGTASK_PLUGIN);
  org::esb::core::PluginRegistry::getInstance()->load(DOWNLOADTASK_PLUGIN);
  org::esb::core::PluginRegistry::getInstance()->load(EXPORTTASK_PLUGIN);
  org::esb::core::PluginRegistry::getInstance()->load(EXECUTABLETASK_PLUGIN);
  org::esb::core::PluginRegistry::getInstance()->load(UPLOADTASK_PLUGIN);
  org::esb::core::PluginRegistry::getInstance()->initPlugins();
  boost::shared_ptr<db::HiveDb> database = boost::shared_ptr<db::HiveDb>(new db::HiveDb("sqlite3", org::esb::config::Config::get("db.url")));

  org::esb::io::File graph_file(std::string(MEC_SOURCE_DIR) + "/test-data/test.graph");
  if (graph_file.exists()) {
    std::string graphdata;
    org::esb::io::FileInputStream fis(&graph_file);
    fis.read(graphdata);
    if (libjson::is_valid(graphdata)) {
      LOGDEBUG("Data is valid");
      JSONNode inode = libjson::parse(graphdata);
      graph::GraphVerifier * v = graph::GraphVerifier::getInstance();
      if (!v->verify(inode, database)) {
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

