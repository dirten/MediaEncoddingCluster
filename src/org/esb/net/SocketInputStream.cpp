#include "org/esb/io/InputStream.h"
#include "org/esb/net/Socket.h"
#include <iostream>

#if !defined(WIN32) 
    #include <sys/select.h>
    #include <sys/socket.h>
    #include <sys/ioctl.h>
#else
    #include <winsock2.h>
#endif


using namespace org::esb::io;
using namespace org::esb::net;
using namespace std;

//using namespace org::esb::net;
namespace org
{
  namespace esb
  {
    namespace net
    {

      class SocketInputStream: public InputStream
      {
      private:
        Socket * socket;
      public:
        /******************************************************************************/
        ~SocketInputStream()
        {
        
        }
        /******************************************************************************/
        SocketInputStream(Socket * socket)
        {
          this->socket=socket;
        }

        /******************************************************************************/
        int read(unsigned char * buffer, int length)
        {
          int maxrecv=8192;
          int all=0, counter=1, offset=0;
          while(all<length)
          {
	    int readLength=(length-all)<maxrecv?(length-all):maxrecv;
            counter=::recv(this->socket->getDescriptor(),(char*)buffer+offset,readLength,0);
            /*If Connection is dead*/
            if(counter<0)
            {
              this->socket->close();
              return false;
            }
            offset+=counter;
            all+=counter;
          }
          return all;
        }
        
        /******************************************************************************/
        int available(bool isBlocking)
        {


	#if defined(WIN32) 

	    unsigned long numBytes = 0;

	    if (::ioctlsocket (this->socket->getDescriptor(), FIONREAD, &numBytes) == SOCKET_ERROR){
//    		throw SocketException( __FILE__, __LINE__, "ioctlsocket failed" );
	    }

	    return (std::size_t)numBytes;

	#else // !defined(HAVE_WINSOCK2_H)

	#if defined(FIONREAD)
	if(isBlocking){
	    int counter=recv(this->socket->getDescriptor(),NULL,0,0x100);
	    if(counter<0){
		this->socket->close();
	    }
	}
	
        int numBytes = 0;
	    if( ::ioctl (this->socket->getDescriptor(), FIONREAD, &numBytes) != -1 ){
	    return numBytes;
        }
	#endif
	#endif
          return 1;
        }
      };
    }
  }
}

