#include "SqlException.h"

#include "org/esb/util/Log.h"

namespace org{
namespace esb{
namespace sql{
	SqlException::SqlException(std::string msg, int err):message(msg){
		_error_code=err;
		logerror(msg<<"("<<_error_code<<")");
	}
	const char * SqlException::what(){
		return message.c_str();
	}
	/*
SqlException::SqlException(const char * msg):Exception("",0,msg){
	logerror(msg);
}
*/
}}}



