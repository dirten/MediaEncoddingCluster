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
	    int remaining=len, byteCounter=0, sendOpts = MSG_NOSIGNAL;
	    while(remaining>0){
    		int bytes=::send(this->socket->getDescriptor(),buffer,remaining,sendOpts);
		byteCounter+=bytes;
		if(bytes<0){
		    cout << "Fehler beim versenden"<< endl;
           	    this->socket->close();
		    throw "fehler beim versenden";
		}
		buffer+=bytes;
		remaining-=bytes;
	    }
	}
};
}}}


