#include "org/esb/io/InputStream.h"
#include "org/esb/net/Socket.h"
#include <iostream>

#if !defined(WIN32) 
    #include <sys/select.h>
    #include <sys/socket.h>
    #include <sys/ioctl.h>
    #include <errno.h>
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
        int read(unsigned char * buffer, int length)throw (org::esb::lang::Exception)
        {
          int  counter=1;
            	counter=::recv(this->socket->getDescriptor(),(char*)buffer,length,0);
            	/*If Connection is dead*/
				cout << "error:"<<errno<<"\t strerror"<<strerror(errno)<<"\t counter:"<<counter<<endl;
            	if(counter<=0){
              		this->socket->close();
					throw Exception( __FILE__, __LINE__, "socket is unusable");
            	}
          return counter;
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
	    int counter=::recv(this->socket->getDescriptor(),NULL,0,MSG_PEEK);
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

