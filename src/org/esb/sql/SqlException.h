#ifndef ORG_ESB_SQL_SQLEXCEPTION_H
#define ORG_ESB_SQL_SQLEXCEPTION_H

#include <string>
#include <stdexcept>
#include "org/esb/util/Log.h"

//#include <boost/exception.hpp>
namespace org{
namespace esb{
namespace sql{
class SqlException : /*public boost::exception, */public std::runtime_error {
	logger("hive.sql")
	public:
		explicit SqlException(std::string & msg);
		explicit SqlException(const char * msg);

};
}}}
#endif

