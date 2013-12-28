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
#include "org/esb/core/PluginContext.h"
#include "org/esb/hive/Environment.h"
#include "org/esb/hive/HiveClient.h"
#include "org/esb/io/File.h"
#include "org/esb/io/FileInputStream.h"
#include "org/esb/config/config.h"
//#include "org/esb/mq/QueueConnection.h"
//#include "org/esb/mq/QueueMessageListener.h"
//#include "org/esb/mq/ObjectMessage.h"

//#include "plugins/services/partitionservice/PartitionManager.h"
//#include "plugins/services/partitionservice/ProcessUnitCollector.h"

#include <iostream>
using namespace std;
using namespace org::esb::core;
using namespace std;
bool toexit = false;

void print_status(Graph * g) {
    std::cerr << g->getStatus();
}
void process(boost::asio::ip::tcp::endpoint e1) {

}

/*
 * 
 */
int main(int argc, char** argv) {
  Log::open();
  if (argc < 3) {
    std::cout << "usage: " << argv[0] << " <graph_path> <input_file>" << std::endl;
	return 1;
  }

  org::esb::hive::Environment::build(argc, argv);
  org::esb::core::PluginRegistry::getInstance()->load(ENCODINGTASK_PLUGIN);
  org::esb::core::PluginRegistry::getInstance()->load(DOWNLOADTASK_PLUGIN);
  org::esb::core::PluginRegistry::getInstance()->load(EXPORTTASK_PLUGIN);
  org::esb::core::PluginRegistry::getInstance()->load(EXECUTABLETASK_PLUGIN);
  org::esb::core::PluginRegistry::getInstance()->load(UPLOADTASK_PLUGIN);
  org::esb::core::PluginRegistry::getInstance()->load(OUTPUTTASK_PLUGIN);
  org::esb::core::PluginRegistry::getInstance()->load(HTTPPULLTASK_PLUGIN);
  org::esb::core::PluginRegistry::getInstance()->initPlugins();
  org::esb::core::PluginRegistry::getInstance()->startServerServices();
  //LOGDEBUG("HttpPullSource:"<<HTTPPULLTASK_PLUGIN);
  //org::esb::core::PluginRegistry::getInstance()->load(HTTPPULLTASK_PLUGIN);
  LOGDEBUG("using database in:" << org::esb::config::Config::get("db.url"));
  boost::shared_ptr<db::HiveDb> database = boost::shared_ptr<db::HiveDb > (new db::HiveDb("sqlite3", org::esb::config::Config::get("db.url")));
  /*Loading flow from disk*/
  std::string flow(argv[1]);
  LOGDEBUG("loading flow:" << flow);
  org::esb::io::File flowfile(flow);
  if(!flowfile.exists()){
	  LOGERROR("flow file not found:"<<flowfile.getPath());
	  return 1;
  }
  org::esb::io::FileInputStream fis(flow);
  std::string graph_data;
  fis.read(graph_data);

  GraphParser graphparser(graph_data,argv[2]);
  
  
  GraphParser::ElementMap & el = graphparser.getElementMap();
  std::list<Ptr<Graph::Element> > list;

  foreach(GraphParser::ElementMap::value_type & element, el) {
    element.second->task->getContext()->set<std::string>("uuid","0815");
    list.push_back(element.second);
  }

  /*
  boost::thread t1 = go(process, e1, col);
  boost::thread t2 = go(process, e2, col);
  boost::thread t3 = go(process, e3, col);
  boost::thread t4 = go(process, e4, col);
  boost::thread t5 = go(process, e5, col);
  */
/*  
  con2.setMessageListener("read_q",listener2);
  con2.startListener();
  con3.setMessageListener("read_q",listener3);
  con3.startListener();
  con4.setMessageListener("read_q",listener4);
  con4.startListener();
  */ 
  Graph graph(list, "0815");
  graph.addStatusObserver(boost::bind(&print_status,_1));
  //boost::thread t6 = go(print_status, &graph);
  graph.run();
  toexit = true;
/*
  t1.join();
  t2.join();
  t3.join();
  t4.join();
  t5.join();
*/
  std::cerr << graph.getStatus();

  return 0;
}

