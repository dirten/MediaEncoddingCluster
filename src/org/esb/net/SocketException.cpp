#include <string.h>
#include "SocketException.h"




using namespace org::esb::net;

SocketException::SocketException(std::string & msg):org::esb::lang::Exception(msg){
  //LOGERROR(msg.c_str());
}
SocketException::SocketException(const char * msg):org::esb::lang::Exception(msg){
  //LOGERROR(msg);
}


