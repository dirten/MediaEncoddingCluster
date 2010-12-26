/* 
 * File:   ProtocolDispatcher.cpp
 * Author: HoelscJ
 * 
 * Created on 1. Oktober 2010, 14:32
 */

#include "ProtocolDispatcher.h"
#include "org/esb/util/Log.h"
#include "ServerHandler.h"
namespace org {
  namespace esb {
    namespace grid {

      ProtocolDispatcher::ProtocolDispatcher(Ptr<org::esb::net::TcpSocket> s):_socket(s) {
        _handler_list.push_back(new ServerHandler(s));
      }

      ProtocolDispatcher::~ProtocolDispatcher() {
      }

      void ProtocolDispatcher::dispatch(std::string datacmd) {

      }
      
      void ProtocolDispatcher::run() {
        while(_socket->isConnected()){
          LOGDEBUG("waiting for command");
          string cmd;
          int dataLength = _socket->getInputStream()->read(cmd);
          if(dataLength==0)continue;
          LOGDEBUG("Command="<<cmd);
          std::list<Ptr<NodeCmdHandler> >::iterator dspit=_handler_list.begin();
          for(;dspit!=_handler_list.end();dspit++){
            if((*dspit)->handleCommand(cmd))
              break;
          }
        }
      }
    }
  }
}
