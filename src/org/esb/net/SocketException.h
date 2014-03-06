#ifndef ORG_ESB_NET_SOCKETEXCEPTION_H
#define ORG_ESB_NET_SOCKETEXCEPTION_H

#include <string.h>
#include <stdexcept>

#include "org/esb/lang/Exception.h"
#include "exports.h"

namespace org{
namespace esb{
namespace net{
class NET_EXPORT SocketException : public org::esb::lang::Exception {

  public:
		explicit SocketException(std::string & msg);
		explicit SocketException(const char * msg);
};
}}}
#endif

