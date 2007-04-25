#ifndef ORG_ESB_NET_SERVERSOCKET_H
#define ORG_ESB_NET_SERVERSOCKET_H
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "org/esb/net/socket.h"

#if defined( __APPLE__ )
#define SOCKET_NOSIGNAL SO_NOSIGPIPE
#elif defined( unix ) && !defined( __CYGWIN__ ) && !defined( sun )
#define SOCKET_NOSIGNAL MSG_NOSIGNAL
#else
#define SOCKET_NOSIGNAL 0
#endif
using namespace org::esb::net;
namespace org
{
  namespace esb
  {
    namespace net
    {
      class ServerSocket
      {
      private:
        int   port;
        char  * hostname;
        int   socketFd;
        int   connectFd;
        struct sockaddr_in socketaddr;
        void  init();
      public:
        ServerSocket(int);
        ServerSocket();
        ~ServerSocket();

        void        setHostname(char*);
        void        setPort(int);
        char        * getHostname();
        int         getPort();
        void        bind();
        Socket*     accept();
        void        close();
      };
    }
  }
}
#endif
