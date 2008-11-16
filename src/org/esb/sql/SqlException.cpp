#include "SqlException.h"

#include "org/esb/util/Log.h"

namespace org{
namespace esb{
namespace sql{
	SqlException::SqlException(std::string & msg):Exception("",0,msg.c_str())/*std::runtime_error(msg)*/{
	logerror(msg);
}
SqlException::SqlException(const char * msg):Exception("",0,msg)/*std::runtime_error(msg)*/{
	logerror(msg);
}
}}}



