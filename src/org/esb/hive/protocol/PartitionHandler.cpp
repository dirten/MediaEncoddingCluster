/* 
 * File:   PartitionHandler.cpp
 * Author: jhoelscher
 * 
 * Created on 14. August 2011, 15:22
 */

#include "PartitionHandler.h"
#include <string>
namespace org {
  namespace esb {
    namespace hive {

      PartitionHandler::PartitionHandler(org::esb::io::InputStream * is, org::esb::io::OutputStream * os, boost::asio::ip::tcp::endpoint e) : _is(is), _os(os), _endpoint(e) {
      }

      PartitionHandler::~PartitionHandler() {
      }

      int PartitionHandler::isResponsible(char * command) {
        std::string cmd=command;
        if(cmd==JOIN_PARTITION||cmd==LEAVE_PARTITION)
          return CMD_PROCESS;
        return CMD_NONE;
      }

      void PartitionHandler::process(char * command) {
        
      }
    }
  }
}

