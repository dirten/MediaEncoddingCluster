#include "org/esb/io/OutputStream.h"
#include "Socket.h"
//#include <exception.h>
using namespace org::esb::io;
using namespace std;
namespace org{
namespace esb{
namespace net{
class SocketOutputStream:public OutputStream{
    private:
	Socket * socket;
    public:
/******************************************************************************/
	~SocketOutputStream(){
	}
/******************************************************************************/
	SocketOutputStream(Socket * socket){
	    this->socket=socket;
	}

/******************************************************************************/
	void write(vector<unsigned char>&buffer){
	    write((char*)&buffer[0], buffer.size());
	}
/******************************************************************************/
	void write(char * buffer, int len){
	    if(this->socket->isClosed()){
		cout << "Socket is Closed"<<endl;
//		return;
	    }
	    int remaining=len, byteCounter=0, sendOpts = MSG_NOSIGNAL, bytes=0;

	    /*
	    * Send length from buffer
	    */
	    bytes=::send(this->socket->getDescriptor(),&len,sizeof(int),sendOpts);

	    /*
	    * Send buffer
	    */
//	    cout << "neuer sendevorgang mit der länge:"<<len<<endl;
//	    sendOpts=0;
	    while(remaining>0){
    		bytes=::send(this->socket->getDescriptor(),buffer,remaining,sendOpts);
		byteCounter+=bytes;
//		cout << "ByteCounter:"<<byteCounter<<endl;
		if(bytes<0){
		    perror("error send packet");
		    cout << "Fehler beim versenden"<< endl;
//           	    this->socket->close();
//		    throw "fehler beim versenden";
		}
		buffer+=bytes;
		remaining-=bytes;
	    }
	}
};
}}}


