/* 
 * File:   ServerCommand.cpp
 * Author: HoelscJ
 * 
 * Created on 31. Dezember 2010, 13:03
 */

#include "ServerCommand.h"
namespace org {
  namespace esb {
    namespace grid {

      ServerCommand::ServerCommand(Type type) {
        header.messageType=0;
        if(type==START){
          operation="start";
        }else{
          operation="stop";
        }
      }

      ServerCommand::~ServerCommand() {
      }
    }
  }
}
