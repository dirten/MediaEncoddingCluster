#include "org/esb/io/InputStream.h"
#include "org/esb/net/Socket.h"
#include <iostream>
#include <sys/ioctl.h>
using namespace org::esb::io;
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
        SocketInputStream(Socket * socket)
        {
          this->socket=socket;
        }

        /******************************************************************************/
        int read(unsigned char * buffer, int length)
        {
          int maxrecv=8192;
          char recvBuffer[maxrecv];
          int all=0, counter=1, offset=0;
          while(all<length)
          {
            counter=recv(this->socket->getDescriptor(),recvBuffer,maxrecv,NULL);
            /*If Connection is dead*/
            if(counter<0)
            {
              this->socket->close();
              return false;
            }
            memcpy(buffer+offset,recvBuffer,counter);
            offset+=counter;
            all+=counter;
          }
          return all;
        }
        
        /******************************************************************************/
        int available(bool isBlocking)
        {

	#if defined(FIONREAD)
	if(isBlocking)
	    int counter=recv(this->socket->getDescriptor(),NULL,0,MSG_PEEK);
        int numBytes = 0;
	    if( ::ioctl (this->socket->getDescriptor(), FIONREAD, &numBytes) != -1 ){
	    cout << "Bytes Available:"<<numBytes<<endl;
	    return numBytes;
        }
	#endif
          return 1;
        }
      };
    }
  }
}

