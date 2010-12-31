/* 
 * File:   ProtocolDispatcher.cpp
 * Author: HoelscJ
 * 
 * Created on 1. Oktober 2010, 14:32
 */

#include "ProtocolDispatcher.h"
#include "org/esb/util/Log.h"
#include "ServerHandler.h"
#include "org/esb/grid/command/Command.h"
namespace org {
  namespace esb {
    namespace grid {

      ProtocolDispatcher::ProtocolDispatcher(Ptr<org::esb::net::TcpSocket> s):_socket(s) {
        _handler_list.push_back(new ServerHandler(s));
        _ois=new org::esb::io::ObjectInputStream(_socket->getInputStream());
      }

      ProtocolDispatcher::~ProtocolDispatcher() {
      }

      void ProtocolDispatcher::dispatch(std::string datacmd) {

      }
      
      void ProtocolDispatcher::run() {
        while(_socket->isConnected()){
          LOGDEBUG("waiting for command");
          
          //string cmd;
          org::esb::grid::Command * cmd=new org::esb::grid::Command();
          
          if(_ois->readObject(cmd)!=0)continue;

          LOGDEBUG("Command="<<typeid(cmd).name());
          std::list<Ptr<NodeCmdHandler> >::iterator dspit=_handler_list.begin();
          for(;dspit!=_handler_list.end();dspit++){
            if((*dspit)->handleCommand(cmd->operation))
              break;
          }
        }
      }
    }
  }
}
