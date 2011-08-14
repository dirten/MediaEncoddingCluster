/* 
 * File:   PartitionManager.cpp
 * Author: HoelscJ
 * 
 * Created on 18. Juli 2011, 09:56
 */

#include "PartitionManager.h"
namespace org {
  namespace esb {
    namespace hive {
      PartitionManager * PartitionManager::_instance = 0;

      PartitionManager * PartitionManager::getInstance() {
        if (_instance == 0)
          _instance = new PartitionManager();
        return _instance;
      }

      PartitionManager::PartitionManager() {
      }

      PartitionManager::~PartitionManager() {
      }

      PartitionManager::Result PartitionManager::joinPartition(std::string name, boost::asio::ip::tcp::endpoint ep) {

      }

      PartitionManager::Result PartitionManager::leavePartition(std::string name, boost::asio::ip::tcp::endpoint ep) {

      }

      PartitionManager::Result PartitionManager::createPartition(std::string name, int size) {
        PartitionManager::Result result = PartitionManager::OK;
        if (_partition_map.find(name) != _partition_map.end()) {
          _partition_map[name];
        }
        else
          result=PartitionManager::EXIST;
        return result;
      }

      PartitionManager::Result PartitionManager::deletePartition(std::string name) {
        PartitionManager::Result result = PartitionManager::OK;
        if (_partition_map.find(name) != _partition_map.end()) {
          _partition_map.erase(name);
        }
        else
          result=PartitionManager::NOT_EXIST;
        return result;

      }
    }
  }
}
