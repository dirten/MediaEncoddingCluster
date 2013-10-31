#include <string.h>
#include "IllegalFormatException.h"
//#include "org/esb/util/Log.h"


using namespace org::esb::util;

IllegalFormatException::IllegalFormatException(std::string & msg):std::runtime_error(msg){
	//LOGERROR(msg.c_str());
}
IllegalFormatException::IllegalFormatException(const char * msg):std::runtime_error(msg){
	//LOGERROR(msg);
}


