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
		 SqlException(std::string & msg);
		 const char * what();
//		 SqlException(const char * msg);
private:
	std::string message;
};
}}}
#endif

