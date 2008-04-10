#ifndef ORG_ESB_NET_SOCKETEXCEPTION_H
#define ORG_ESB_NET_SOCKETEXCEPTION_H

#include <string.h>
#include <stdexcept>
#include "org/esb/util/Log.h"

namespace org{
namespace esb{
namespace net{
class SocketException : public std::runtime_error {
	logger("hive.net")
	public:
		explicit SocketException(std::string & msg);
		explicit SocketException(const char * msg);
};
}}}
#endif

