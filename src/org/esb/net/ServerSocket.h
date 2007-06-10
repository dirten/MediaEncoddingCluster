#ifndef ORG_ESB_NET_SERVERSOCKET_H
#define ORG_ESB_NET_SERVERSOCKET_H



#if defined(WIN32) 
    #include <winsock2.h>
    #include <ws2tcpip.h> 
    #include <sys/stat.h>
    #define stat _stat
#else
    #include <unistd.h>
    #include <netdb.h>
    #include <fcntl.h>
    #include <sys/file.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <string.h>
#endif
//#include <sys/socket.h>
//#include <netinet/in.h>
//#include <arpa/inet.h>
#include "org/esb/net/Socket.h"

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
        int   server_socketFd;
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
        void        bind()throw (Exception);
        Socket*     accept();
        void        close();
      };
    }
  }
}
#endif
