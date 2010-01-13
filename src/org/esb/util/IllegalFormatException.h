#ifndef ORG_ESB_SQL_SQLEXCEPTION_H
#define ORG_ESB_SQL_SQLEXCEPTION_H

#include <string.h>
#include <stdexcept>


namespace org{
namespace esb{
namespace util{
class IllegalFormatException : public std::runtime_error {
	public:
		explicit IllegalFormatException(std::string & msg);
		explicit IllegalFormatException(const char * msg);

};
}}}
#endif

