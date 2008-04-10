#include <string.h>
#include "SqlException.h"




using namespace org::esb::sql;

SqlException::SqlException(std::string & msg):std::runtime_error(msg){
	logerror(msg);
}
SqlException::SqlException(const char * msg):std::runtime_error(msg){
	logerror(msg);
}


