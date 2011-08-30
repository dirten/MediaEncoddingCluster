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
#include "org/esb/util/Log.h"
using namespace std;

void test_simple_create_and_delete() {
  org::esb::hive::PartitionManager *man = org::esb::hive::PartitionManager::getInstance();
  assert(man->createPartition("test") == org::esb::hive::PartitionManager::OK);
  assert(man->createPartition("test") == org::esb::hive::PartitionManager::EXIST);
  assert(man->deletePartition("test") == org::esb::hive::PartitionManager::OK);
  assert(man->deletePartition("test") == org::esb::hive::PartitionManager::NOT_EXIST);
}

void test_simple_join_and_leave() {
  org::esb::hive::PartitionManager *man = org::esb::hive::PartitionManager::getInstance();
  assert(man->createPartition("test") == org::esb::hive::PartitionManager::OK);
  boost::asio::ip::tcp::endpoint e(boost::asio::ip::address_v4::from_string("127.0.0.1"), 6000);
  org::esb::hive::PartitionManager::Result result = man->joinPartition("test", e);
  assert(result == org::esb::hive::PartitionManager::OK);
  assert(man->joinPartition("test", e) == org::esb::hive::PartitionManager::ENDPOINT_ALLREADY_JOINED);
  assert(man->joinPartition("global", e) == org::esb::hive::PartitionManager::ENDPOINT_ALLREADY_JOINED);
  assert(man->joinPartition("global-test", e) == org::esb::hive::PartitionManager::NOT_EXIST);
  assert(man->leavePartition("test", e) == org::esb::hive::PartitionManager::OK);
  assert(man->leavePartition("test", e) == org::esb::hive::PartitionManager::NOT_IN_PARTITION);
  assert(man->deletePartition("test") == org::esb::hive::PartitionManager::OK);
}

void test_partitioning() {
  org::esb::hive::PartitionManager *man = org::esb::hive::PartitionManager::getInstance();
  assert(man->createPartition("test") == org::esb::hive::PartitionManager::OK);
  boost::asio::ip::tcp::endpoint e1(boost::asio::ip::address_v4::from_string("127.0.0.1"), 6000);
  boost::asio::ip::tcp::endpoint e2(boost::asio::ip::address_v4::from_string("127.0.0.1"), 6001);
  boost::asio::ip::tcp::endpoint e3(boost::asio::ip::address_v4::from_string("127.0.0.1"), 6002);
  boost::asio::ip::tcp::endpoint e4(boost::asio::ip::address_v4::from_string("127.0.0.1"), 6003);

  assert(man->joinPartition("test", e1, org::esb::hive::PartitionManager::TYPE_VIDEO) == org::esb::hive::PartitionManager::OK);
  assert(man->joinPartition("test", e2, org::esb::hive::PartitionManager::TYPE_VIDEO) == org::esb::hive::PartitionManager::OK);
  assert(man->joinPartition("test", e3, org::esb::hive::PartitionManager::TYPE_AUDIO) == org::esb::hive::PartitionManager::OK);
  assert(man->joinPartition("test", e4, org::esb::hive::PartitionManager::TYPE_AUDIO) == org::esb::hive::PartitionManager::OK);

  boost::shared_ptr<org::esb::hive::job::ProcessUnit> pu1 = boost::shared_ptr<org::esb::hive::job::ProcessUnit > (new org::esb::hive::job::ProcessUnit());
  boost::shared_ptr<org::esb::hive::job::ProcessUnit> pu2 = boost::shared_ptr<org::esb::hive::job::ProcessUnit > (new org::esb::hive::job::ProcessUnit());
  boost::shared_ptr<org::esb::hive::job::ProcessUnit> pu3 = boost::shared_ptr<org::esb::hive::job::ProcessUnit > (new org::esb::hive::job::ProcessUnit());

  {

    pu1->_source_stream = 1;
    pu2->_source_stream = 2;
    pu3->_source_stream = 3;

    int a = 0;
    pu1->_last_process_unit = false;
    pu1->_process_unit = ++a;
    man->putProcessUnit("test", pu1, org::esb::hive::PartitionManager::TYPE_VIDEO);
    pu1->_process_unit = ++a;
    man->putProcessUnit("test", pu1, org::esb::hive::PartitionManager::TYPE_VIDEO);
    pu1->_process_unit = ++a;
    pu1->_last_process_unit = true;
    man->putProcessUnit("test", pu1, org::esb::hive::PartitionManager::TYPE_VIDEO);
    //pu1->_process_unit=++a;man->putProcessUnit("test",pu1, org::esb::hive::PartitionManager::TYPE_VIDEO);
    pu2->_process_unit = ++a;
    pu2->_last_process_unit = false;
    man->putProcessUnit("test", pu2, org::esb::hive::PartitionManager::TYPE_AUDIO);
    pu2->_process_unit = ++a;
    man->putProcessUnit("test", pu2, org::esb::hive::PartitionManager::TYPE_AUDIO);
    pu2->_process_unit = ++a;
    pu2->_last_process_unit = true;
    man->putProcessUnit("test", pu2, org::esb::hive::PartitionManager::TYPE_AUDIO);

    pu3->_last_process_unit = false;
    pu3->_process_unit = ++a;
    man->putProcessUnit("test", pu3, org::esb::hive::PartitionManager::TYPE_AUDIO);
    pu3->_process_unit = ++a;
    man->putProcessUnit("test", pu3, org::esb::hive::PartitionManager::TYPE_AUDIO);
    pu3->_last_process_unit = true;
    pu3->_process_unit = ++a;
    man->putProcessUnit("test", pu3, org::esb::hive::PartitionManager::TYPE_AUDIO);
    //assert(man->getProcessUnit(e1)->_source_stream==1);
    boost::shared_ptr<org::esb::hive::job::ProcessUnit> gu11 = man->getProcessUnit(e1);
    assert(gu11->_source_stream == 1);
    assert(gu11->_process_unit == 1);

    boost::shared_ptr<org::esb::hive::job::ProcessUnit> gu21 = man->getProcessUnit(e2);
    assert(gu21->_source_stream == 1);
    assert(gu21->_process_unit == 2);

    boost::shared_ptr<org::esb::hive::job::ProcessUnit> gu12 = man->getProcessUnit(e1);
    assert(gu12->_source_stream == 1);
    assert(gu12->_process_unit == 3);

    boost::shared_ptr<org::esb::hive::job::ProcessUnit> gu22 = man->getProcessUnit(e2);
    assert(gu22->_source_stream == 0);
    assert(gu22->_process_unit == 0);

    boost::shared_ptr<org::esb::hive::job::ProcessUnit> gu13 = man->getProcessUnit(e1);
    assert(gu13->_source_stream == 0);
    assert(gu13->_process_unit == 0);

    boost::shared_ptr<org::esb::hive::job::ProcessUnit> gu31 = man->getProcessUnit(e3);
    assert(gu31->_source_stream == 2);
    assert(gu31->_process_unit == 4);


    boost::shared_ptr<org::esb::hive::job::ProcessUnit> gu41 = man->getProcessUnit(e4);
    assert(gu41->_source_stream == 3);
    assert(gu41->_process_unit == 7);

    boost::shared_ptr<org::esb::hive::job::ProcessUnit> gu32 = man->getProcessUnit(e3);
    assert(gu32->_source_stream == 2);
    assert(gu32->_process_unit == 5);

    boost::shared_ptr<org::esb::hive::job::ProcessUnit> gu42 = man->getProcessUnit(e4);
    assert(gu42->_source_stream == 3);
    assert(gu42->_process_unit == 8);

    boost::shared_ptr<org::esb::hive::job::ProcessUnit> gu33 = man->getProcessUnit(e3);
    assert(gu33->_source_stream == 2);
    assert(gu33->_process_unit == 6);

    boost::shared_ptr<org::esb::hive::job::ProcessUnit> gu43 = man->getProcessUnit(e4);
    assert(gu43->_source_stream == 3);
    assert(gu43->_process_unit == 9);

    boost::shared_ptr<org::esb::hive::job::ProcessUnit> gu34 = man->getProcessUnit(e3);
    assert(gu34->_source_stream == 0);
    assert(gu34->_process_unit == 0);

    boost::shared_ptr<org::esb::hive::job::ProcessUnit> gu44 = man->getProcessUnit(e4);
    assert(gu44->_source_stream == 0);
    assert(gu44->_process_unit == 0);
  }

  {
    pu1->_source_stream = 4;
    pu2->_source_stream = 5;
    pu3->_source_stream = 6;

    int a = 0;
    LOGDEBUG("begin round 2")
    pu1->_last_process_unit = false;
    pu1->_process_unit = ++a;
    man->putProcessUnit("test", pu1, org::esb::hive::PartitionManager::TYPE_VIDEO);
    pu1->_process_unit = ++a;
    man->putProcessUnit("test", pu1, org::esb::hive::PartitionManager::TYPE_VIDEO);
    pu1->_last_process_unit = true;
    pu1->_process_unit = ++a;
    man->putProcessUnit("test", pu1, org::esb::hive::PartitionManager::TYPE_VIDEO);
    //pu1->_process_unit=++a;man->putProcessUnit("test",pu1, org::esb::hive::PartitionManager::TYPE_VIDEO);
    pu2->_last_process_unit = false;
    pu2->_process_unit = ++a;
    man->putProcessUnit("test", pu2, org::esb::hive::PartitionManager::TYPE_AUDIO);
    pu2->_process_unit = ++a;
    man->putProcessUnit("test", pu2, org::esb::hive::PartitionManager::TYPE_AUDIO);
    pu2->_last_process_unit = true;
    pu2->_process_unit = ++a;
    man->putProcessUnit("test", pu2, org::esb::hive::PartitionManager::TYPE_AUDIO);

    pu3->_last_process_unit = false;
    pu3->_process_unit = ++a;
    man->putProcessUnit("test", pu3, org::esb::hive::PartitionManager::TYPE_AUDIO);
    pu3->_process_unit = ++a;
    man->putProcessUnit("test", pu3, org::esb::hive::PartitionManager::TYPE_AUDIO);
    pu3->_last_process_unit = true;
    pu3->_process_unit = ++a;
    man->putProcessUnit("test", pu3, org::esb::hive::PartitionManager::TYPE_AUDIO);

    boost::shared_ptr<org::esb::hive::job::ProcessUnit> gu11 = man->getProcessUnit(e1);
    LOGDEBUG(gu11->_source_stream);
    LOGDEBUG(gu11->_process_unit);
    assert(gu11->_source_stream == 4);
    assert(gu11->_process_unit == 1);

    boost::shared_ptr<org::esb::hive::job::ProcessUnit> gu21 = man->getProcessUnit(e2);
    LOGDEBUG(gu21->_source_stream);
    LOGDEBUG(gu21->_process_unit);
    assert(gu21->_source_stream == 4);
    assert(gu21->_process_unit == 2);

    boost::shared_ptr<org::esb::hive::job::ProcessUnit> gu12 = man->getProcessUnit(e1);
    assert(gu12->_source_stream == 4);
    assert(gu12->_process_unit == 3);

    boost::shared_ptr<org::esb::hive::job::ProcessUnit> gu22 = man->getProcessUnit(e2);
    assert(gu22->_source_stream == 0);
    assert(gu22->_process_unit == 0);

    boost::shared_ptr<org::esb::hive::job::ProcessUnit> gu13 = man->getProcessUnit(e1);
    assert(gu13->_source_stream == 0);
    assert(gu13->_process_unit == 0);

    boost::shared_ptr<org::esb::hive::job::ProcessUnit> gu31 = man->getProcessUnit(e3);
    assert(gu31->_source_stream == 5);
    assert(gu31->_process_unit == 4);


    boost::shared_ptr<org::esb::hive::job::ProcessUnit> gu41 = man->getProcessUnit(e4);
    assert(gu41->_source_stream == 6);
    assert(gu41->_process_unit == 7);

    boost::shared_ptr<org::esb::hive::job::ProcessUnit> gu32 = man->getProcessUnit(e3);
    assert(gu32->_source_stream == 5);
    assert(gu32->_process_unit == 5);

    boost::shared_ptr<org::esb::hive::job::ProcessUnit> gu42 = man->getProcessUnit(e4);
    assert(gu42->_source_stream == 6);
    assert(gu42->_process_unit == 8);

    boost::shared_ptr<org::esb::hive::job::ProcessUnit> gu33 = man->getProcessUnit(e3);
    assert(gu33->_source_stream == 5);
    assert(gu33->_process_unit == 6);

    boost::shared_ptr<org::esb::hive::job::ProcessUnit> gu43 = man->getProcessUnit(e4);
    assert(gu43->_source_stream == 6);
    assert(gu43->_process_unit == 9);

    boost::shared_ptr<org::esb::hive::job::ProcessUnit> gu34 = man->getProcessUnit(e3);
    assert(gu34->_source_stream == 0);
    assert(gu34->_process_unit == 0);

    boost::shared_ptr<org::esb::hive::job::ProcessUnit> gu44 = man->getProcessUnit(e4);
    assert(gu44->_source_stream == 0);
    assert(gu44->_process_unit == 0);
  }
}

/*
 * 
 */
int main(int argc, char** argv) {
  Log::open();
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

