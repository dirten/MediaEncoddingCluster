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
	SocketOutputStream(Socket * socket){
	    this->socket=socket;
	}

/******************************************************************************/
	void write(char * buffer, int len){
	    int remaining=len, byteCounter=0, sendOpts = SOCKET_NOSIGNAL;
	    cout << "OutputStreamLength"<<len<<endl;
	    while(remaining>0){
//    		int bytes=::send(this->socket->getDescriptor(),buffer,remaining,sendOpts);
    		int bytes=::write(this->socket->getDescriptor(),buffer,remaining);
		byteCounter+=bytes;
		if(bytes<0){
		    this->socket->close();
		}
		buffer+=bytes;
		remaining-=bytes;
	    }
	    cout << "Gesendet"<<byteCounter<<endl;
	}
};
}}}


