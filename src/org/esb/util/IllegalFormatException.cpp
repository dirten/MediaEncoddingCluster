#include <string.h>
#include "IllegalFormatException.h"




using namespace org::esb::util;

IllegalFormatException::IllegalFormatException(std::string & msg):std::runtime_error(msg){
	logerror(msg);
}
IllegalFormatException::IllegalFormatException(const char * msg):std::runtime_error(msg){
	logerror(msg);
}


