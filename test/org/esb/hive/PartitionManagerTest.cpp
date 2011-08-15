/* 
 * File:   PartitionManagerTest.cpp
 * Author: jhoelscher
 *
 * Created on 14. August 2011, 16:20
 */

#include <cstdlib>
#include <iostream>
#include "org/esb/hive/job/PartitionManager.h"
using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
  org::esb::hive::PartitionManager *man = org::esb::hive::PartitionManager::getInstance();
  std::cout <<"Result Create"<<man->createPartition("test", 4)<<std::endl;
  boost::asio::ip::tcp::endpoint e(boost::asio::ip::address_v4::from_string("127.0.0.1"), 6000);
  for (int a = 0; a < 5; a++){
    org::esb::hive::PartitionManager::Result r= man->joinPartition("test", e);
    if ( r== org::esb::hive::PartitionManager::OK)
      std::cout << "partition joined" << std::endl;
    else
      std::cout << "partion join failed"<<r << std::endl;
  }
  
  return 0;
}

