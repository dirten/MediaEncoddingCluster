#ifndef ORG_ESB_NET_SOCKETEXCEPTION_H
#define ORG_ESB_NET_SOCKETEXCEPTION_H

#include <string.h>
#include <stdexcept>


namespace org{
namespace esb{
namespace net{
class SocketException : public std::runtime_error {
	public:
		explicit SocketException(std::string & msg);
		explicit SocketException(const char * msg);
};
}}}
#endif

