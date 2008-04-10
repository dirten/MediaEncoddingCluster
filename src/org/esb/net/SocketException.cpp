#include <string.h>
#include "SocketException.h"




using namespace org::esb::net;

SocketException::SocketException(std::string & msg):std::runtime_error(msg){
	logerror(msg);
}
SocketException::SocketException(const char * msg):std::runtime_error(msg){
	logerror(msg);
}


