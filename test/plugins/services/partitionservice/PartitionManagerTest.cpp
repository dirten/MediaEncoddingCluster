/* 
 * File:   PartitionManagerTest.cpp
 * Author: jhoelscher
 *
 * Created on 14. August 2011, 16:20
 */

#include <cstdlib>
#include <iostream>
#include "plugins/services/partitionservice/PartitionManager.h"
#include "org/esb/lang/Thread.h"
#include "org/esb/util/Log.h"
#include "org/esb/config/config.h"
using namespace std;

void test_simple_create_and_delete() {
  partitionservice::PartitionManager *man = partitionservice::PartitionManager::getInstance();
  assert(man->createPartition("test") == partitionservice::PartitionManager::OK);
  assert(man->createPartition("test") == partitionservice::PartitionManager::EXIST);
  assert(man->deletePartition("test") == partitionservice::PartitionManager::OK);
  assert(man->deletePartition("test") == partitionservice::PartitionManager::NOT_EXIST);
}

void test_simple_join_and_leave() {
  partitionservice::PartitionManager *man = partitionservice::PartitionManager::getInstance();
  assert(man->createPartition("test") == partitionservice::PartitionManager::OK);
  boost::asio::ip::tcp::endpoint e(boost::asio::ip::address_v4::from_string("127.0.0.1"), 6000);
  partitionservice::PartitionManager::Result result = man->joinPartition("test", e);
  assert(result == partitionservice::PartitionManager::OK);
  assert(man->joinPartition("test", e) == partitionservice::PartitionManager::ENDPOINT_ALLREADY_JOINED);
  assert(man->joinPartition("global", e) == partitionservice::PartitionManager::ENDPOINT_ALLREADY_JOINED);
  assert(man->joinPartition("global-test", e) == partitionservice::PartitionManager::NOT_EXIST);
  assert(man->leavePartition("test", e) == partitionservice::PartitionManager::OK);
  assert(man->leavePartition("test", e) == partitionservice::PartitionManager::NOT_IN_PARTITION);
  assert(man->deletePartition("test") == partitionservice::PartitionManager::OK);
}

void test_partitioning() {
  partitionservice::PartitionManager *man = partitionservice::PartitionManager::getInstance();
  assert(man->createPartition("test") == partitionservice::PartitionManager::OK);
  boost::asio::ip::tcp::endpoint e1(boost::asio::ip::address_v4::from_string("127.0.0.1"), 6000);
  boost::asio::ip::tcp::endpoint e2(boost::asio::ip::address_v4::from_string("127.0.0.1"), 6001);
  boost::asio::ip::tcp::endpoint e3(boost::asio::ip::address_v4::from_string("127.0.0.1"), 6002);
  boost::asio::ip::tcp::endpoint e4(boost::asio::ip::address_v4::from_string("127.0.0.1"), 6003);

  assert(man->joinPartition("test", e1, partitionservice::PartitionManager::TYPE_VIDEO) == partitionservice::PartitionManager::OK);
  assert(man->joinPartition("test", e2, partitionservice::PartitionManager::TYPE_VIDEO) == partitionservice::PartitionManager::OK);
  assert(man->joinPartition("test", e3, partitionservice::PartitionManager::TYPE_AUDIO) == partitionservice::PartitionManager::OK);
  assert(man->joinPartition("test", e4, partitionservice::PartitionManager::TYPE_AUDIO) == partitionservice::PartitionManager::OK);

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
    man->putProcessUnit("test", pu1, partitionservice::PartitionManager::TYPE_VIDEO);
    pu1->_process_unit = ++a;
    man->putProcessUnit("test", pu1, partitionservice::PartitionManager::TYPE_VIDEO);
    pu1->_process_unit = ++a;
    pu1->_last_process_unit = true;
    man->putProcessUnit("test", pu1, partitionservice::PartitionManager::TYPE_VIDEO);
    //pu1->_process_unit=++a;man->putProcessUnit("test",pu1, org::esb::hive::PartitionManager::TYPE_VIDEO);
    pu2->_process_unit = ++a;
    pu2->_last_process_unit = false;
    man->putProcessUnit("test", pu2, partitionservice::PartitionManager::TYPE_AUDIO);
    pu2->_process_unit = ++a;
    man->putProcessUnit("test", pu2, partitionservice::PartitionManager::TYPE_AUDIO);
    pu2->_process_unit = ++a;
    pu2->_last_process_unit = true;
    man->putProcessUnit("test", pu2, partitionservice::PartitionManager::TYPE_AUDIO);

    pu3->_last_process_unit = false;
    pu3->_process_unit = ++a;
    man->putProcessUnit("test", pu3, partitionservice::PartitionManager::TYPE_AUDIO);
    pu3->_process_unit = ++a;
    man->putProcessUnit("test", pu3, partitionservice::PartitionManager::TYPE_AUDIO);
    pu3->_last_process_unit = true;
    pu3->_process_unit = ++a;
    man->putProcessUnit("test", pu3, partitionservice::PartitionManager::TYPE_AUDIO);
    //assert(man->getProcessUnit(e1)->_source_stream==1);
    boost::shared_ptr<org::esb::hive::job::ProcessUnit> gu11 = man->getProcessUnit(e1);
     //gu11 = man->getProcessUnit(e1);
     //gu11 = man->getProcessUnit(e1);
    LOGDEBUG(gu11->_process_unit);
    assert(gu11->_source_stream == 1);
    assert(gu11->_process_unit == 1);

    boost::shared_ptr<org::esb::hive::job::ProcessUnit> gu21 = man->getProcessUnit(e2);
    assert(gu21->_source_stream == 2);
    assert(gu21->_process_unit == 4);

    boost::shared_ptr<org::esb::hive::job::ProcessUnit> gu12 = man->getProcessUnit(e1);
    assert(gu12->_source_stream == 1);
    assert(gu12->_process_unit == 2);

    boost::shared_ptr<org::esb::hive::job::ProcessUnit> gu22 = man->getProcessUnit(e2);
    assert(gu22->_source_stream == 2);
    assert(gu22->_process_unit == 5);

    boost::shared_ptr<org::esb::hive::job::ProcessUnit> gu13 = man->getProcessUnit(e1);
    assert(gu13->_source_stream == 1);
    assert(gu13->_process_unit == 3);

    boost::shared_ptr<org::esb::hive::job::ProcessUnit> gu31 = man->getProcessUnit(e3);
    assert(gu31->_source_stream == 3);
    assert(gu31->_process_unit == 7);


    boost::shared_ptr<org::esb::hive::job::ProcessUnit> gu41 = man->getProcessUnit(e2);
    
    assert(gu41->_source_stream == 2);
    assert(gu41->_process_unit == 6);

    boost::shared_ptr<org::esb::hive::job::ProcessUnit> gu32 = man->getProcessUnit(e3);
    assert(gu32->_source_stream == 3);
    assert(gu32->_process_unit == 8);

    boost::shared_ptr<org::esb::hive::job::ProcessUnit> gu42 = man->getProcessUnit(e4);
    assert(!gu42);
    //assert(gu42->_source_stream == 3);
    //assert(gu42->_process_unit == 8);

    boost::shared_ptr<org::esb::hive::job::ProcessUnit> gu33 = man->getProcessUnit(e3);
    assert(gu33->_source_stream == 3);
    assert(gu33->_process_unit == 9);

    boost::shared_ptr<org::esb::hive::job::ProcessUnit> gu43 = man->getProcessUnit(e4);
    assert(!gu43);
    //assert(gu43->_source_stream == 3);
    //assert(gu43->_process_unit == 9);

    boost::shared_ptr<org::esb::hive::job::ProcessUnit> gu34 = man->getProcessUnit(e3);
    assert(!gu34);
    //assert(gu34->_source_stream == 0);
    //assert(gu34->_process_unit == 0);

    boost::shared_ptr<org::esb::hive::job::ProcessUnit> gu44 = man->getProcessUnit(e4);
    assert(!gu44);
    //assert(gu44->_source_stream == 0);
    //assert(gu44->_process_unit == 0);
  }
  man->leavePartition("", e1);
  man->leavePartition("", e2);
  man->leavePartition("", e2);
  man->leavePartition("", e3);
  man->leavePartition("", e4);
  return;
  {
    pu1->_source_stream = 4;
    pu2->_source_stream = 5;
    pu3->_source_stream = 6;

    int a = 0;
    LOGDEBUG("begin round 2")
    pu1->_last_process_unit = false;
    pu1->_process_unit = ++a;
    man->putProcessUnit("test", pu1, partitionservice::PartitionManager::TYPE_VIDEO);
    pu1->_process_unit = ++a;
    man->putProcessUnit("test", pu1, partitionservice::PartitionManager::TYPE_VIDEO);
    pu1->_last_process_unit = true;
    pu1->_process_unit = ++a;
    man->putProcessUnit("test", pu1, partitionservice::PartitionManager::TYPE_VIDEO);
    //pu1->_process_unit=++a;man->putProcessUnit("test",pu1, org::esb::hive::PartitionManager::TYPE_VIDEO);
    pu2->_last_process_unit = false;
    pu2->_process_unit = ++a;
    man->putProcessUnit("test", pu2, partitionservice::PartitionManager::TYPE_AUDIO);
    pu2->_process_unit = ++a;
    man->putProcessUnit("test", pu2, partitionservice::PartitionManager::TYPE_AUDIO);
    pu2->_last_process_unit = true;
    pu2->_process_unit = ++a;
    man->putProcessUnit("test", pu2, partitionservice::PartitionManager::TYPE_AUDIO);

    pu3->_last_process_unit = false;
    pu3->_process_unit = ++a;
    man->putProcessUnit("test", pu3, partitionservice::PartitionManager::TYPE_AUDIO);
    pu3->_process_unit = ++a;
    man->putProcessUnit("test", pu3, partitionservice::PartitionManager::TYPE_AUDIO);
    pu3->_last_process_unit = true;
    pu3->_process_unit = ++a;
    man->putProcessUnit("test", pu3, partitionservice::PartitionManager::TYPE_AUDIO);

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


void endpoint_reader(boost::asio::ip::tcp::endpoint ep, int sid){
  LOGDEBUG("starting endpoint_reader for endpoint:"<<ep<<" and stream:"<<sid);
  partitionservice::PartitionManager *man = partitionservice::PartitionManager::getInstance();

  bool action=true;
  boost::shared_ptr<org::esb::hive::job::ProcessUnit> pu;
  do{
    pu = man->getProcessUnit(ep);
    if(pu){
        //assert(pu->_source_stream == sid);
        action=false;
        LOGDEBUG("Receive ProcessUnit:"<<pu->_process_unit);
        org::esb::lang::Thread::sleep2(sid*10);
        man->collectProcessUnit(pu, ep);
    }
  }while(pu);
  LOGDEBUG("leaving endpoint_reader for endpoint:"<<ep<<" and stream:"<<sid);
}

void test_partitioning_2() {
  boost::asio::ip::tcp::endpoint e1(boost::asio::ip::address_v4::from_string("127.0.0.1"), 6000);
  boost::asio::ip::tcp::endpoint e2(boost::asio::ip::address_v4::from_string("127.0.0.1"), 6001);
  boost::asio::ip::tcp::endpoint e3(boost::asio::ip::address_v4::from_string("127.0.0.1"), 6002);
  boost::asio::ip::tcp::endpoint e4(boost::asio::ip::address_v4::from_string("127.0.0.1"), 6003);
  
  
    boost::shared_ptr<org::esb::hive::job::ProcessUnit> pu = boost::shared_ptr<org::esb::hive::job::ProcessUnit > (new org::esb::hive::job::ProcessUnit());
  partitionservice::PartitionManager *man = partitionservice::PartitionManager::getInstance();

    pu->_source_stream = 1;

    int a = 0;
    for(;a<1000;a++){
      pu->_last_process_unit = false;
      pu->_process_unit = a;
      man->putProcessUnit("test", pu, partitionservice::PartitionManager::TYPE_VIDEO);      
    }
    pu->_last_process_unit = true;
    pu->_process_unit = a;
    man->putProcessUnit("test", pu, partitionservice::PartitionManager::TYPE_VIDEO);      
    
    
    pu->_source_stream = 2;
    for(;a<1000*2;a++){
      pu->_last_process_unit = false;
      pu->_process_unit = a;
      man->putProcessUnit("test", pu, partitionservice::PartitionManager::TYPE_AUDIO);      
    }
    pu->_last_process_unit = true;
    pu->_process_unit = a;
    man->putProcessUnit("test", pu, partitionservice::PartitionManager::TYPE_AUDIO);      

    pu->_source_stream = 3;
    for(;a<1000*3;a++){
      pu->_last_process_unit = false;
      pu->_process_unit = a;
      man->putProcessUnit("test", pu, partitionservice::PartitionManager::TYPE_AUDIO);      
    }
    pu->_last_process_unit = true;
    pu->_process_unit = a;
    man->putProcessUnit("test", pu, partitionservice::PartitionManager::TYPE_AUDIO);      
    /*
    pu->_source_stream = 4;
    for(;a<1000*4;a++){
      pu->_last_process_unit = false;
      pu->_process_unit = a;
      man->putProcessUnit("test", pu, partitionservice::PartitionManager::TYPE_AUDIO);      
    }
    pu->_last_process_unit = true;
    pu->_process_unit = a;
    man->putProcessUnit("test", pu, partitionservice::PartitionManager::TYPE_AUDIO);      
    pu->_source_stream = 5;
    for(;a<1000*5;a++){
      pu->_last_process_unit = false;
      pu->_process_unit = a;
      man->putProcessUnit("test", pu, partitionservice::PartitionManager::TYPE_AUDIO);      
    }
    pu->_last_process_unit = true;
    pu->_process_unit = a;
    man->putProcessUnit("test", pu, partitionservice::PartitionManager::TYPE_AUDIO);      
    pu->_source_stream = 6;
    for(;a<1000*6;a++){
      pu->_last_process_unit = false;
      pu->_process_unit = a;
      man->putProcessUnit("test", pu, partitionservice::PartitionManager::TYPE_AUDIO);      
    }
    pu->_last_process_unit = true;
    pu->_process_unit = a;
    man->putProcessUnit("test", pu, partitionservice::PartitionManager::TYPE_AUDIO);      
*/
  
  go(endpoint_reader, e1, 1);
  go(endpoint_reader, e2, 2);
  go(endpoint_reader, e3, 3);
  go(endpoint_reader, e4, 4);
  
  while(partitionservice::PartitionManager::getInstance()->getSize("test")>0)
        org::esb::lang::Thread::sleep2(1000);
}


/*
 * 
 */
int main(int argc, char** argv) {
  Log::open();
  org::esb::config::Config::setProperty("hive.tmp_path",".");
  
  org::esb::lang::Thread::sleep2(500);
  test_simple_create_and_delete();
  test_simple_join_and_leave();
  test_partitioning();
  test_partitioning_2();
  return 0;

  partitionservice::PartitionManager *man = partitionservice::PartitionManager::getInstance();
  std::cout << "Result Create" << man->createPartition("test", 4) << std::endl;
  boost::asio::ip::tcp::endpoint e(boost::asio::ip::address_v4::from_string("127.0.0.1"), 6000);
  for (int a = 0; a < 5; a++) {
    partitionservice::PartitionManager::Result r = man->joinPartition("test", e);
    if (r == partitionservice::PartitionManager::OK)
      std::cout << "partition joined" << std::endl;
    else
      std::cout << "partion join failed" << r << std::endl;
  }
  
  return 0;
}

