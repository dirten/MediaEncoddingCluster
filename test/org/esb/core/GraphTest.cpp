/* 
 * File:   GraphTest.cpp
 * Author: jhoelscher
 *
 * Created on 12. Dezember 2011, 10:08
 */

#include <cstdlib>
#include <list>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include "org/esb/core/Graph.h"
#include "org/esb/core/GraphParser.h"
#include "org/esb/util/Log.h"
#include "org/esb/util/Foreach.h"
#include "org/esb/core/PluginRegistry.h"
#include "org/esb/hive/Environment.h"
#include "org/esb/hive/HiveClient.h"
#include "org/esb/io/File.h"
#include "org/esb/io/FileInputStream.h"
#include "org/esb/config/config.h"

#include "plugins/services/partitionservice/PartitionManager.h"
#include "plugins/services/partitionservice/ProcessUnitCollector.h"

#include <iostream>
using namespace std;
using namespace org::esb::core;
using namespace std;
bool toexit = false;

void print_status(Graph * g) {
    std::cerr << g->getStatus();
}

void process(boost::asio::ip::tcp::endpoint e1, partitionservice::ProcessUnitCollector & col) {
  boost::shared_ptr<org::esb::hive::job::ProcessUnit> pu;
  partitionservice::PartitionManager * man = partitionservice::PartitionManager::getInstance();
  org::esb::hive::HiveClient client("",0);
  do {
    pu = man->getProcessUnit(e1);
    if (pu) {

      client.processUnit(pu);
      man->collectProcessUnit(pu, e1);
      //col.putProcessUnit(pu);
    } else {
      org::esb::lang::Thread::sleep2(1000);
    }
  } while (!toexit);

}

/*
 * 
 */
int main(int argc, char** argv) {
  Log::open();
  if (argc < 3) {
    std::cout << "usage: " << argv[0] << " <graph_path> <input_file>" << std::endl;
  }

  org::esb::hive::Environment::build(argc, argv);
  org::esb::core::PluginRegistry::getInstance()->load(ENCODINGTASK_PLUGIN);
  org::esb::core::PluginRegistry::getInstance()->load(DOWNLOADTASK_PLUGIN);
  org::esb::core::PluginRegistry::getInstance()->load(EXPORTTASK_PLUGIN);
  org::esb::core::PluginRegistry::getInstance()->load(EXECUTABLETASK_PLUGIN);
  org::esb::core::PluginRegistry::getInstance()->load(UPLOADTASK_PLUGIN);
  org::esb::core::PluginRegistry::getInstance()->load(OUTPUTTASK_PLUGIN);
  LOGDEBUG("using database in:" << org::esb::config::Config::get("db.url"));
  boost::shared_ptr<db::HiveDb> database = boost::shared_ptr<db::HiveDb > (new db::HiveDb("sqlite3", org::esb::config::Config::get("db.url")));

  /*Loading flow from disk*/
  org::esb::io::FileInputStream fis(argv[1]);
  std::string graph_data;
  fis.read(graph_data);
  GraphParser graphparser(graph_data,argv[2]);
  
  
  GraphParser::ElementMap & el = graphparser.getElementMap();
  std::list<Ptr<Graph::Element> > list;

  foreach(GraphParser::ElementMap::value_type & element, el) {
    list.push_back(element.second);
  }

  /*create encoding threads*/
  partitionservice::PartitionManager * man = partitionservice::PartitionManager::getInstance();
  boost::asio::ip::tcp::endpoint e1(boost::asio::ip::address_v4::from_string("127.0.0.1"), 6000);
  boost::asio::ip::tcp::endpoint e2(boost::asio::ip::address_v4::from_string("127.0.0.1"), 6001);
  boost::asio::ip::tcp::endpoint e3(boost::asio::ip::address_v4::from_string("127.0.0.1"), 6002);
  boost::asio::ip::tcp::endpoint e4(boost::asio::ip::address_v4::from_string("127.0.0.1"), 6003);
  boost::asio::ip::tcp::endpoint e5(boost::asio::ip::address_v4::from_string("127.0.0.1"), 6004);

  assert(man->joinPartition("global", e1, partitionservice::PartitionManager::TYPE_VIDEO) == partitionservice::PartitionManager::OK);
  assert(man->joinPartition("global", e5, partitionservice::PartitionManager::TYPE_VIDEO) == partitionservice::PartitionManager::OK);
  assert(man->joinPartition("global", e2, partitionservice::PartitionManager::TYPE_AUDIO) == partitionservice::PartitionManager::OK);
  assert(man->joinPartition("global", e3, partitionservice::PartitionManager::TYPE_AUDIO) == partitionservice::PartitionManager::OK);
  assert(man->joinPartition("global", e4, partitionservice::PartitionManager::TYPE_AUDIO) == partitionservice::PartitionManager::OK);
  partitionservice::ProcessUnitCollector col("collector");
  boost::thread t1 = go(process, e1, col);
  /*
  boost::thread t2 = go(process, e2, col);
  boost::thread t3 = go(process, e3, col);
  boost::thread t4 = go(process, e4, col);
  boost::thread t5 = go(process, e5, col);
  */
  Graph graph(list, "0815");
  graph.addStatusObserver(boost::bind(&print_status,_1));
  //boost::thread t6 = go(print_status, &graph);
  graph.run();

  while (man->getSize("global") > 0) {
    org::esb::lang::Thread::sleep2(1 * 1000);
  }
  toexit = true;
  t1.join();
/*
  t2.join();
  t3.join();
  t4.join();
  t5.join();
*/
  std::cerr << graph.getStatus();

  return 0;
}

