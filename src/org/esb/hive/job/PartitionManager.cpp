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
        PartitionManager::Result result = PartitionManager::OK;
        if(_partition_map.count(name)>0){
          if(_partition_sizes[name]>_partition_map[name].size()){
          _partition_map[name].push_back(ep);
          }else{
            result=PartitionManager::FULL;
          }
        }else{
          result=PartitionManager::NOT_EXIST;
        }
        return result;
      }

      PartitionManager::Result PartitionManager::leavePartition(std::string name, boost::asio::ip::tcp::endpoint ep) {
        PartitionManager::Result result = PartitionManager::OK;
        
        return result;
      }

      PartitionManager::Result PartitionManager::createPartition(std::string name, int size) {
        PartitionManager::Result result = PartitionManager::OK;
        if (_partition_map.find(name) != _partition_map.end()) {
          _partition_map[name];
          _partition_sizes[name] = size;
        } else
          result = PartitionManager::EXIST;
        return result;
      }

      PartitionManager::Result PartitionManager::deletePartition(std::string name) {
        PartitionManager::Result result = PartitionManager::OK;
        if (_partition_map.find(name) != _partition_map.end()) {
          if (_partition_map[name].size() == 0) {
            _partition_map.erase(name);
          } else {
            result = PartitionManager::NOT_EMPTY;
          }
        } else
          result = PartitionManager::NOT_EXIST;
        return result;

      }
    }
  }
}
