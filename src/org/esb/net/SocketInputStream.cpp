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
            counter=::read(this->socket->getDescriptor(),buffer+offset,readLength);
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

	#if defined(FIONREAD)
	if(isBlocking)
	    int counter=recv(this->socket->getDescriptor(),NULL,0,MSG_WAITALL);
        int numBytes = 0;
	    if( ::ioctl (this->socket->getDescriptor(), FIONREAD, &numBytes) != -1 ){
	    return numBytes;
        }
	#endif
          return 1;
        }
      };
    }
  }
}

