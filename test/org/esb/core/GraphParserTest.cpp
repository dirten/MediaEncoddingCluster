/* 
 * File:   GraphParserTest.cpp
 * Author: HoelscJ
 *
 * Created on 17. Januar 2012, 14:48
 */
#include "org/esb/db/hivedb.hpp"

#include <cstdlib>
#include "org/esb/util/Log.h"
#include "org/esb/core/GraphParser.h"
#include "org/esb/io/File.h"
#include "org/esb/io/FileInputStream.h"
#include "org/esb/hive/Environment.h"
using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
  Log::open();
  if (argc < 2) {
    std::cout << "usage: " << argv[0] << " <graph_path> " << std::endl;
  }
  
  
  org::esb::hive::Environment::build(argc, argv);
  org::esb::core::PluginRegistry::getInstance()->load(ENCODINGTASK_PLUGIN);
  org::esb::core::PluginRegistry::getInstance()->load(DOWNLOADTASK_PLUGIN);
  org::esb::core::PluginRegistry::getInstance()->load(EXPORTTASK_PLUGIN);
  org::esb::core::PluginRegistry::getInstance()->load(EXECUTABLETASK_PLUGIN);
  org::esb::core::PluginRegistry::getInstance()->load(UPLOADTASK_PLUGIN);
  org::esb::core::PluginRegistry::getInstance()->load(OUTPUTTASK_PLUGIN);

  org::esb::io::FileInputStream fis(argv[1]);
  std::string graph_data;
  fis.read(graph_data);
  org::esb::core::GraphParser graphparser(graph_data);
  graphparser.setInfile("bla.fasel");
  LOGDEBUG("Graph:"<<graphparser.getGraphString());
  return 0;
}

