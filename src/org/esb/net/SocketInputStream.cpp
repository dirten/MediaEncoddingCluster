#include "org/esb/io/InputStream.h"
#include "TcpSocket.h"
#include <iostream>
#include "org/esb/lang/Byte.h"
//#include "../config.h"
#include <vector>
#if !defined(WIN32) 
/*
#include <sys/select.h>
    #include <sys/socket.h>
    #include <sys/ioctl.h>
    #include <errno.h>
	*/
#else
    #include <winsock2.h>
#endif

#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
using boost::asio::ip::tcp; 

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
		boost::system::error_code error;
        Socket * socket;
		boost::shared_ptr<tcp::socket> _socket;
        char byte;
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
        }
        /******************************************************************************/

		SocketInputStream(Socket * socket)
        {

          this->socket=socket;
		  byte=-1;

        }
		SocketInputStream(boost::shared_ptr<tcp::socket> socket):_socket(socket)
        {

//          this->socket=socket;
//		  byte=-1;

        }

        /******************************************************************************/

		int read(vector<unsigned char>&buffer)
        {
	    size_t size=buffer.size();
	    buffer.clear();
	    unsigned char * tmp_buffer=new unsigned char[size];
	    size_t counter=read(tmp_buffer, size);
            for( std::size_t ix = 0; ix < counter; ++ix )
            {
                buffer.push_back(tmp_buffer[ix]);
            }
			delete tmp_buffer;
	    return counter;
        }

        /******************************************************************************/
        int read()
        {
			read((unsigned char*)&byte,1);
	    	return byte;
        }
        
        /******************************************************************************/
        int read(unsigned char * buffer, int length){
	    int  counter=0;
            /*Receive data into buffer*/
		counter=_socket->read_some(boost::asio::buffer(buffer, length),error);
//		counter=::recv(this->socket->getDescriptor(),(char*)buffer,length,SOCKET_WAITALL);
//            counter=::recv(this->socket->getDescriptor(),(char*)buffer,length,0);
            /*If Connection is dead*/
        /*    
			if(counter<=0){
				cout << "Socket is brocken"<<endl;
              	this->socket->close();
            }
		*/
          return counter;
        }
        
        int read(string & str){
		    int length=available(true);
//	    cout << "Readed Buffer length"<<length<<endl;
			unsigned char * buffer=new unsigned char[length];
	    int  counter=0;
            /*Receive data into buffer*/
//            counter=read(this->socket->getDescriptor(),(char*)buffer,length,SOCKET_WAITALL);
            counter=read((unsigned char*)buffer,length);
            /*If Connection is dead*/
            if(counter<=0){
				cout << "Socket is brocken"<<endl;
              	this->socket->close();
            }else{
        		str=string((char*)buffer, length);
            }
            delete []buffer;
          return counter;
        }
        
        /******************************************************************************/
/*Receive length of buffer*/
		int available(bool isBlocking){
			char tmp[10];
			_socket->read_some(boost::asio::buffer(tmp),error);
			if (error == boost::asio::error::eof)
				return 0;
			else
				throw boost::system::system_error(error);
	    	return atoi(tmp);
        }
      };
    }
  }
}

