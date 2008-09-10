#include <string.h>
#include "SocketException.h"




using namespace org::esb::net;

SocketException::SocketException(std::string & msg):std::runtime_error(msg){
	logerror(msg.c_str());
}
SocketException::SocketException(const char * msg):std::runtime_error(msg){
	logerror(msg);
}


