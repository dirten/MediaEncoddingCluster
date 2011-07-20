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

      PartitionManager PartitionManager::getInstance() {
        return _instance;
      }

      PartitionManager::PartitionManager() {
      }

      PartitionManager::~PartitionManager() {
      }
    }
  }
}
