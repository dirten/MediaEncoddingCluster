#ifndef ORG_ESB_SQL_SQLEXCEPTION_H
#define ORG_ESB_SQL_SQLEXCEPTION_H

#include <string>
#include <stdexcept>
#include "org/esb/lang/Exception.h"


//#include <boost/exception.hpp>
namespace org{
namespace esb{
namespace sql{
class SqlException {
	public:
		 SqlException(std::string & msg, int err=0);
		 const char * what();
		 int getErrorCode(){return _error_code;}
//		 SqlException(const char * msg);
private:
	std::string message;
	int _error_code;
};
}}}
#endif

