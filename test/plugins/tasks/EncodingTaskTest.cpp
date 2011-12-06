/* 
 * File:   DownloadTaskTest.cpp
 * Author: HoelscJ
 *
 * Created on 19. Oktober 2011, 14:03
 */
#include "org/esb/db/hivedb.hpp"
#include <cstdlib>
#include "org/esb/core/PluginRegistry.h"
#include "org/esb/core/PluginContext.h"
#include "org/esb/core/Task.h"

#include "org/esb/util/Log.h"
#include "org/esb/io/FileInputStream.h"
#include "org/esb/config/config.h"
#include <boost/asio.hpp>
#include "plugins/services/partitionservice/PartitionManager.h"
#include "plugins/services/partitionservice/ProcessUnitCollector.h"
#include "org/esb/lang/Thread.h"
#include "org/esb/util/UUID.h"
#include "org/esb/hive/Environment.h"
using namespace std;
bool toexit=false;
void process(boost::asio::ip::tcp::endpoint e1, partitionservice::ProcessUnitCollector & col) {
  boost::shared_ptr<org::esb::hive::job::ProcessUnit> pu;
  partitionservice::PartitionManager * man = partitionservice::PartitionManager::getInstance();
  do {
    pu = man->getProcessUnit(e1);
    if (pu) {
      //if(pu->getDecoder()->getCodecType()==AVMEDIA_TYPE_AUDIO)
      //  org::esb::lang::Thread::sleep2(10*1000);
      pu->process();
      pu->_input_packets.clear();
      man->collectProcessUnit(pu, e1);
      //col.putProcessUnit(pu);
    }else{
      org::esb::lang::Thread::sleep2(1000);
    }
  } while (!toexit);

}

/*
 * 
 */
int main(int argc, char** argv) {
  org::esb::hive::Environment::build(argc,argv);
  Log::open();
  if(argc<3){
    std::cout << "usage: "<<argv[0]<<" <inputfile_path> <profile_path> "<<std::endl;
  }
    

  /*Loading profile from disk*/
  org::esb::io::FileInputStream fis(argv[2]);
  std::string profile_data;
  fis.read(profile_data);
  org::esb::core::PluginRegistry::getInstance()->load(ENCODINGTASK_PLUGIN);
  org::esb::core::PluginRegistry::getInstance()->initPlugins();
  std::map<std::string, std::string> cfg;
  //cfg["encodingtask.src"]=std::string(MEC_SOURCE_DIR).append("/test.dvd");
  cfg["encodingtask.src"]=std::string(argv[1]);
  //cfg["encodingtask.src"] = "/media/video/ChocolateFactory.ts";
  cfg["encodingtask.profiledata"] = profile_data;
  cfg["task.uuid"] = "0815";
  db::HiveDb database("sqlite3", "database=test.db");
  database.drop();
  database.create();
  db::Job job(database);
  job.uuid = (std::string)org::esb::util::PUUID();
  job.update();

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
  boost::thread t1=go(process, e1, col);
  
  boost::thread t2=go(process, e2, col);
  boost::thread t3=go(process, e3, col);
  boost::thread t4=go(process, e4, col);
  boost::thread t5=go(process, e5, col);
  
  
  {
    Ptr<org::esb::core::Task> task = org::esb::core::PluginRegistry::getInstance()->createTask("EncodingTask", cfg);
    //task->getContext()->_props["job"] = job;
    if (task) {
      task->prepare();
      task->execute();
      task->cleanup();
    }
  }
  //return 0;
  while (man->getSize("global") > 0) {
    org::esb::lang::Thread::sleep2(1 * 1000);
  }
  toexit=true;
  t1.join();
  
  t2.join();
  t3.join();
  t4.join();
  t5.join();
   
  /*encoding is ready*/
  std::map<std::string, std::string> expcfg;
  //cfg["encodingtask.src"]=std::string(MEC_SOURCE_DIR).append("/test.dvd");
  expcfg["exporttask.trg"] = "/tmp/ChocolateFactory.mp4";
  expcfg["exporttask.format"] = "mp4";
  expcfg["task.uuid"] = "0815";
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
  database.drop();
  return 0;
}

