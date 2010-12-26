/* 
 * File:   ServerHandler.cpp
 * Author: HoelscJ
 * 
 * Created on 22. Dezember 2010, 13:01
 */

#include "ServerHandler.h"
#include "org/esb/util/Log.h"
namespace org {
  namespace esb {
    namespace grid {

      ServerHandler::ServerHandler(Ptr<org::esb::net::TcpSocket> s):NodeCmdHandler(s) {

      }

      bool ServerHandler::handleCommand(std::string & cmddata){
        LOGDEBUG("handle command: "<<cmddata);
        return false;
      }
      
      ServerHandler::~ServerHandler() {
      }
    }
  }
}
