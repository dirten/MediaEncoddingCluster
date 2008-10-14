#include "org/esb/io/OutputStream.h"
#include "TcpSocket.h"
#include "SocketException.h"
#include <errno.h>

//#include <exception.h>
using namespace org::esb::io;
using namespace std;
namespace org{
namespace esb{
namespace net{
class SocketOutputStream:public OutputStream{
    private:
	TcpSocket * socket;
    public:
/******************************************************************************/
	~SocketOutputStream(){
	}
/******************************************************************************/
	SocketOutputStream(TcpSocket * socket){
	    this->socket=socket;
	}

/******************************************************************************/
	void write(vector<unsigned char>&buffer){
	    write((char*)&buffer[0], buffer.size());
	}
	void write(string&buffer){
	    write((char*)buffer.c_str(), buffer.length());
	}
/******************************************************************************/
	void write(char * buffer, int len){
	    if(this->socket->isClosed()){
//		    string error="SocketOutputStream - Socket is Closed";		    
		    throw SocketException("SocketOutputStream::write - can not Write, because Socket is allready Closed");
//		cout << "Socket is Closed"<<endl;
//		return;
	    }
	    int remaining=len, byteCounter=0, bytes=0;

	    /*
	    * Send length from buffer
	    */
	    bytes=::send(this->socket->getDescriptor(),(const char *)&len,sizeof(int),SOCKET_NOSIGNAL);

	    /*
	    * Send buffer
	    */
	    while(remaining>0){
    		bytes=::send(this->socket->getDescriptor(),buffer,remaining,SOCKET_NOSIGNAL);
		byteCounter+=bytes;
		if(bytes<0){
		    string error="SocketOutputStream::write - ";
		    error+=strerror(errno);
           	    this->socket->close();
		    throw SocketException(error);
		}
		buffer+=bytes;
		remaining-=bytes;
	    }
	}
};
}}}


