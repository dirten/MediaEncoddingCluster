#include "org/esb/io/InputStream.h"
#include "org/esb/net/Socket.h"
#include <iostream>
#include "org/esb/lang/Byte.h"
#include "../config.h"
#include <vector>
#if !defined(WIN32) 
    #include <sys/select.h>
    #include <sys/socket.h>
    #include <sys/ioctl.h>
    #include <errno.h>
#else
    #include <winsock2.h>
#endif


using namespace org::esb::lang;
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
        uint8_t byte;
        /**
         * The mutex object.
         */
        #ifdef HAVE_PTHREAD_H
            pthread_mutex_t mutex;
        #else
            CRITICAL_SECTION mutex;
        #endif
/*
	#if defined(WIN32) 
	#define Synchronized
	#else
	#endif
*/	
      public:
        /******************************************************************************/
        ~SocketInputStream()
        {
		#ifdef HAVE_PTHREAD_H
    		pthread_mutex_destroy(&mutex);
		#else
    		DeleteCriticalSection(&mutex);
		#endif
        }
        /******************************************************************************/
        SocketInputStream(Socket * socket)
        {
		#ifdef HAVE_PTHREAD_H
    		pthread_mutexattr_t attr;
    		pthread_mutexattr_init(&attr);
    		pthread_mutex_init(&mutex, &attr);
    		pthread_mutexattr_destroy(&attr);
		#else
    		InitializeCriticalSection(&mutex);            
		#endif
          this->socket=socket;
		  byte=-1;

        }

        /******************************************************************************/
        int read(vector<unsigned char>&buffer)
        {
	    size_t size=buffer.size();
	    buffer.clear();
	    unsigned char tmp_buffer[size];
	    size_t counter=read(tmp_buffer, size);
            for( std::size_t ix = 0; ix < counter; ++ix )
            {
                buffer.push_back(tmp_buffer[ix]);
            }
	    return counter;
        }

        /******************************************************************************/
        int read()
        {
		read((unsigned char*)&byte,1);
	    	return byte;
        }
        
        /******************************************************************************/
        int read(unsigned char * buffer, int length)
        {
	    int  counter=0, len=0;
            /*Receive data into buffer*/
            counter=::recv(this->socket->getDescriptor(),(char*)buffer,length,MSG_WAITALL);
            /*If Connection is dead*/
            if(counter<=0){
		cout << "Socket is brocken"<<endl;
              	this->socket->close();
            }
          return counter;
        }
        
        int read(string & str)
        {
	    int length=available(true);
//	    cout << "Readed Buffer length"<<length<<endl;
	    char *buffer=new char[length];
//	    char buffer[length];
	    int  counter=0, len=0;
            /*Receive data into buffer*/
//            counter=::recv(this->socket->getDescriptor(),(char*)buffer,length,MSG_WAITALL);
            counter=read((unsigned char*)buffer,length);
            /*If Connection is dead*/
            if(counter<=0){
		cout << "Socket is brocken"<<endl;
              	this->socket->close();
            }else{
        	str=string(buffer, length);
            }
            delete buffer;
          return counter;
        }
        /******************************************************************************/
        int available(bool isBlocking){
	    int numBytes = 0, len=0;
	    if(isBlocking){
		/*Receive length of buffer*/
        	numBytes=::recv(this->socket->getDescriptor(),(char*)&len,sizeof(int),MSG_WAITALL);
		if(numBytes<0){
		    this->socket->close();
		}
	    }
	    return len;
        }
      };
    }
  }
}

