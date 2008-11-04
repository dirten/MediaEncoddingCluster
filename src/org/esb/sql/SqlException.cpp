#include "SqlException.h"



namespace org{
namespace esb{
namespace sql{
SqlException::SqlException(std::string & msg):Exception::Exception("",0,msg.c_str())/*std::runtime_error(msg)*/{
	logerror(msg);
}
SqlException::SqlException(const char * msg):Exception::Exception("",0,msg)/*std::runtime_error(msg)*/{
	logerror(msg);
}
}}}



