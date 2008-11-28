#include "SqlException.h"

#include "org/esb/util/Log.h"

namespace org{
namespace esb{
namespace sql{
	SqlException::SqlException(std::string & msg):message(msg){
		logerror(msg);
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



