/* 
 * File:   PartitionManagerTest.cpp
 * Author: jhoelscher
 *
 * Created on 14. August 2011, 16:20
 */

#include <cstdlib>
#include <iostream>
#include "org/esb/hive/job/PartitionManager.h"
#include "org/esb/mq/QueueManager.h"
#include "org/esb/lang/Thread.h"
using namespace std;

void test_simple_create_and_delete() {
  org::esb::hive::PartitionManager *man = org::esb::hive::PartitionManager::getInstance();
  assert(man->createPartition("test")==org::esb::hive::PartitionManager::OK);
  assert(man->createPartition("test")==org::esb::hive::PartitionManager::EXIST);
  assert(man->deletePartition("test")==org::esb::hive::PartitionManager::OK);
  assert(man->deletePartition("test")==org::esb::hive::PartitionManager::NOT_EXIST);  
}

void test_simple_join_and_leave() {
  org::esb::hive::PartitionManager *man = org::esb::hive::PartitionManager::getInstance();
  assert(man->createPartition("test")==org::esb::hive::PartitionManager::OK);
  boost::asio::ip::tcp::endpoint e(boost::asio::ip::address_v4::from_string("127.0.0.1"), 6000);
  assert(man->joinPartition("test", e) == org::esb::hive::PartitionManager::OK);
  assert(man->joinPartition("test", e) == org::esb::hive::PartitionManager::ENDPOINT_ALLREADY_JOINED);
  assert(man->joinPartition("global", e) == org::esb::hive::PartitionManager::ENDPOINT_ALLREADY_JOINED);
  assert(man->joinPartition("global-test", e) == org::esb::hive::PartitionManager::NOT_EXIST);
  assert(man->leavePartition("test", e) == org::esb::hive::PartitionManager::OK);
  assert(man->leavePartition("test", e) == org::esb::hive::PartitionManager::NOT_IN_PARTITION);
  assert(man->deletePartition("test")==org::esb::hive::PartitionManager::OK);
}
void test_partitioning(){
  org::esb::hive::PartitionManager *man = org::esb::hive::PartitionManager::getInstance();
  assert(man->createPartition("test")==org::esb::hive::PartitionManager::OK);
  boost::asio::ip::tcp::endpoint e1(boost::asio::ip::address_v4::from_string("127.0.0.1"), 6000);
  boost::asio::ip::tcp::endpoint e2(boost::asio::ip::address_v4::from_string("127.0.0.1"), 6001);
  boost::asio::ip::tcp::endpoint e3(boost::asio::ip::address_v4::from_string("127.0.0.1"), 6002);
  boost::asio::ip::tcp::endpoint e4(boost::asio::ip::address_v4::from_string("127.0.0.1"), 6003);

  assert(man->joinPartition("test", e1, org::esb::hive::PartitionManager::TYPE_VIDEO) == org::esb::hive::PartitionManager::OK);
  assert(man->joinPartition("test", e2, org::esb::hive::PartitionManager::TYPE_VIDEO) == org::esb::hive::PartitionManager::OK);
  assert(man->joinPartition("test", e3, org::esb::hive::PartitionManager::TYPE_AUDIO) == org::esb::hive::PartitionManager::OK);
  assert(man->joinPartition("test", e4, org::esb::hive::PartitionManager::TYPE_AUDIO) == org::esb::hive::PartitionManager::OK);

  boost::shared_ptr<org::esb::hive::job::ProcessUnit> pu1=boost::shared_ptr<org::esb::hive::job::ProcessUnit>(new org::esb::hive::job::ProcessUnit());
  boost::shared_ptr<org::esb::hive::job::ProcessUnit> pu2=boost::shared_ptr<org::esb::hive::job::ProcessUnit>(new org::esb::hive::job::ProcessUnit());
  boost::shared_ptr<org::esb::hive::job::ProcessUnit> pu3=boost::shared_ptr<org::esb::hive::job::ProcessUnit>(new org::esb::hive::job::ProcessUnit());

  pu1->_source_stream=1;
  pu2->_source_stream=2;
  pu3->_source_stream=3;
  
  man->putProcessUnit("test",pu1, org::esb::hive::PartitionManager::TYPE_VIDEO);
  man->putProcessUnit("test",pu1, org::esb::hive::PartitionManager::TYPE_VIDEO);
  man->putProcessUnit("test",pu1, org::esb::hive::PartitionManager::TYPE_VIDEO);
  man->putProcessUnit("test",pu2, org::esb::hive::PartitionManager::TYPE_AUDIO);
  man->putProcessUnit("test",pu2, org::esb::hive::PartitionManager::TYPE_AUDIO);
  man->putProcessUnit("test",pu2, org::esb::hive::PartitionManager::TYPE_AUDIO);
  man->putProcessUnit("test",pu3, org::esb::hive::PartitionManager::TYPE_AUDIO);
  man->putProcessUnit("test",pu3, org::esb::hive::PartitionManager::TYPE_AUDIO);
  man->putProcessUnit("test",pu3, org::esb::hive::PartitionManager::TYPE_AUDIO);
  
  boost::shared_ptr<org::esb::hive::job::ProcessUnit> gu1=man->getProcessUnit(e1);
  boost::shared_ptr<org::esb::hive::job::ProcessUnit> gu2=man->getProcessUnit(e2);
  boost::shared_ptr<org::esb::hive::job::ProcessUnit> gu3=man->getProcessUnit(e3);
  boost::shared_ptr<org::esb::hive::job::ProcessUnit> gu4=man->getProcessUnit(e4);
  
}
/*
 * 
 */
int main(int argc, char** argv) {
  org::esb::mq::QueueManager qman;
  qman.start();
  org::esb::lang::Thread::sleep2(500);
  test_simple_create_and_delete();
  test_simple_join_and_leave();
  test_partitioning();
  return 0;

  org::esb::hive::PartitionManager *man = org::esb::hive::PartitionManager::getInstance();
  std::cout << "Result Create" << man->createPartition("test", 4) << std::endl;
  boost::asio::ip::tcp::endpoint e(boost::asio::ip::address_v4::from_string("127.0.0.1"), 6000);
  for (int a = 0; a < 5; a++) {
    org::esb::hive::PartitionManager::Result r = man->joinPartition("test", e);
    if (r == org::esb::hive::PartitionManager::OK)
      std::cout << "partition joined" << std::endl;
    else
      std::cout << "partion join failed" << r << std::endl;
  }
  qman.stop();
  return 0;
}

