#include "SqlException.h"



namespace org{
namespace esb{
namespace sql{
SqlException::SqlException(std::string & msg):std::runtime_error(msg){
	logerror(msg);
}
SqlException::SqlException(const char * msg):std::runtime_error(msg){
	logerror(msg);
}
}}}



