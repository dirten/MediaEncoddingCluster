#ifndef ORG_ESB_NET_SOCKET_H
#define ORG_ESB_NET_SOCKET_H
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include "org/esb/io/InputStream.h"
#include "org/esb/io/OutputStream.h"
#if defined( __APPLE__ )
#define SOCKET_NOSIGNAL SO_NOSIGPIPE
#elif defined( unix ) && !defined( __CYGWIN__ ) && !defined( sun )
#define SOCKET_NOSIGNAL MSG_NOSIGNAL
#else
#define SOCKET_NOSIGNAL 0
#endif
using namespace org::esb::io;
namespace org
{
  namespace esb
  {
    namespace net
    {
      struct Socket_t
      {
        char * hostname;
        int port;
      };

      struct SocketData{
        char *data;
        int data_length;
//	Socket *socket;
      };

      class Socket
      {
      private:
        bool is_closed;
        int   port;
        char  * hostname;
        int   socketFd;
        struct sockaddr_in socketaddr;
        void  init();
        InputStream * inputStream;
        OutputStream * outputStream;
        public:
        Socket(int);
        Socket();
        Socket(char * hostname, int portnumber);
        ~Socket();

        void        setHostname(char*);
        void        setPort(int);
        char        * getHostname();
        int         getPort();
        void        bind();
        Socket*     accept();
        void        connect();
        void        close();
	bool	    isClosed();
 int getDescriptor();
	InputStream * getInputStream();
	OutputStream * getOutputStream();
      };

    }
  }
}
#endif
