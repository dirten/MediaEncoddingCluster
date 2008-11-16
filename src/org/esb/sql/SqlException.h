#ifndef ORG_ESB_SQL_SQLEXCEPTION_H
#define ORG_ESB_SQL_SQLEXCEPTION_H

#include <string>
#include <stdexcept>
#include "org/esb/lang/Exception.h"


//#include <boost/exception.hpp>
namespace org{
namespace esb{
namespace sql{
class SqlException : /*public boost::exception, */ /*public std::runtime_error*/public org::esb::lang::Exception {
	public:
		 SqlException(std::string & msg);
		 SqlException(const char * msg);

};
}}}
#endif

