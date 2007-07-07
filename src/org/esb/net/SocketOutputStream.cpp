#include "org/esb/io/OutputStream.h"
#include "Socket.h"
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
	    int remaining=len, byteCounter=0, sendOpts = MSG_NOSIGNAL;
	    while(remaining>0){
    		int bytes=::send(this->socket->getDescriptor(),buffer,remaining,sendOpts);
		byteCounter+=bytes;
		if(bytes<0){
		    return;
		}
		buffer+=bytes;
		remaining-=bytes;
	    }
	}
};
}}}


