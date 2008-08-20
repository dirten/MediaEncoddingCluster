#ifndef ORG_ESB_SQL_SQLEXCEPTION_H
#define ORG_ESB_SQL_SQLEXCEPTION_H

#include <string.h>
#include <stdexcept>
#include "org/esb/util/Log.h"

namespace org{
namespace esb{
namespace util{
class IllegalFormatException : public std::runtime_error {
	logger("esb.util")
	public:
		explicit IllegalFormatException(std::string & msg);
		explicit IllegalFormatException(const char * msg);

};
}}}
#endif

