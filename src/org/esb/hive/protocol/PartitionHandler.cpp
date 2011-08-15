/* 
 * File:   PartitionHandler.cpp
 * Author: jhoelscher
 * 
 * Created on 14. August 2011, 15:22
 */

#include "PartitionHandler.h"
#include "org/esb/hive/job/PartitionManager.h"
#include <string>
namespace org {
  namespace esb {
    namespace hive {

      PartitionHandler::PartitionHandler(org::esb::io::InputStream * is, org::esb::io::OutputStream * os, boost::asio::ip::tcp::endpoint e) : _is(is), _os(os), _endpoint(e) {
        _oos = new io::ObjectOutputStream(os);
        _ois = new io::ObjectInputStream(is);
      }

      PartitionHandler::~PartitionHandler() {
        org::esb::hive::PartitionManager::getInstance()->leavePartition(_partition, _endpoint);
        if (_oos)
          delete _oos;
        _oos = NULL;
        if (_ois)
          delete _ois;
        _ois = NULL;
      }

      int PartitionHandler::isResponsible(cmdId & cmid) {
        return CMD_NA;
      }

      int PartitionHandler::isResponsible(char * command) {
        std::string cmd = command;
        if (cmd == JOIN_PARTITION || cmd == LEAVE_PARTITION)
          return CMD_PROCESS;
        return CMD_NONE;
      }

      void PartitionHandler::process(char * command) {
        std::string cmd = command;
        if (cmd == JOIN_PARTITION) {
          //std::string partition;
          _ois->readObject(_partition);
          std::cout << "JOIN PARTITION " << _partition << std::endl;
          org::esb::hive::PartitionManager::getInstance()->joinPartition(_partition, _endpoint);
        }
        if (cmd == LEAVE_PARTITION) {
          //std::string partition;
          _ois->readObject(_partition);
          std::cout << "LEAVE PARTITION " << _partition << std::endl;
          org::esb::hive::PartitionManager::getInstance()->leavePartition(_partition, _endpoint);
        }
      }
    }
  }
}

