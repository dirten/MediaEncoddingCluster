#include "org/esb/io/InputStream.h"
#include "org/esb/net/Socket.h"
#include <iostream>

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
	  string internalBuffer;
          while(counter!=0)
          {
            counter=recv(this->socket->getDescriptor(),recvBuffer,maxrecv,NULL);
            /*If Connection is dead*/
            if(counter<0)
            {
              this->socket->close();
              return false;
            }
            internalBuffer+=recvBuffer;
            memcpy(buffer+offset,recvBuffer,counter);
            offset+=counter;
            all+=counter;
          }
          return all;
        }

        int read(string * buffer)
        {
          int maxrecv=8192;
          char recvBuffer[maxrecv];

          int all=0, counter=1, offset=0;
//	  string internalBuffer;
          while(counter!=0)
          {
            counter=recv(this->socket->getDescriptor(),recvBuffer,maxrecv,NULL);
            /*If Connection is dead*/
            if(counter<0)
            {
              this->socket->close();
              return false;
            }
            if(counter>0){
        	buffer->append(recvBuffer);
	    }
//            memcpy(buffer+offset,recvBuffer,counter);
            offset+=counter;
            all+=counter;
          }
          return all;
        }


        int available()
        {

          return 1;



        }
      };
    }
  }
}

