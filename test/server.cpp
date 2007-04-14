
#include <iostream>
#include "config.h"
#include "socket.h"
using namespace std;
class server{
    public:
    server(){
	    new EsbConfig("test.cfg");
	    Socket *socket=new Socket();
	    socket->setHostname("localhost");
	    socket->setPort(atoi(EsbConfig::getConfig("port")));
	    socket->Listen();
	for(;;){
	    cout<<"Waiting for connection"<<endl;
	    socket->Accept();
	    cout<<"conection accepted"<<endl;
	    socket->send((unsigned char*)"bla fasel1234");
	    cout<<"sendet"<<endl;
//	    while(true);
//	    socket->close2();
//	    exit(0);
	}
    }
};
int main(int argc,char**argv){
    server * s=new server();
}
