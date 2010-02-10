#include <string.h>
#include "SocketException.h"
#include "org/esb/util/Log.h"



using namespace org::esb::net;

SocketException::SocketException(std::string & msg):std::runtime_error(msg){
	LOGERROR(msg.c_str());
}
SocketException::SocketException(const char * msg):std::runtime_error(msg){
	LOGERROR(msg);
}


