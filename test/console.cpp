#include <iostream>
#include "org/esb/net/Socket.h"
#include "org/esb/io/OutputStream.h"
#include "org/esb/io/InputStream.h"
#include <signal.h>



using namespace std;
using namespace org::esb::net;
int maxline=1000;
Socket * mysocket;

void catcher(int sig){
    cout << "shutdown" << endl;
    mysocket->getOutputStream()->write((unsigned char*)"disconnect",10);
    mysocket->close();
    exit(0);
}
void setCatcher(){
    signal(SIGABRT, &catcher);
    signal(SIGTERM, &catcher);
    signal(SIGQUIT, &catcher);
    signal(SIGHUP, &catcher);
    signal(SIGINT, &catcher);

}
int main(int argc,char**argv){

    setCatcher();
    mysocket=new Socket("localhost", 20000);
    mysocket->connect();
    OutputStream * out=mysocket->getOutputStream();
    InputStream * in=mysocket->getInputStream();
    char buffer[maxline];
    cout << "cmd:> ";

    for(;fgets(buffer, maxline, stdin);){
	cout << "reply:>"<<buffer;
	out->write((const unsigned char*)buffer, strlen(buffer));

	int dataLength=in->available(true);
	unsigned char * inbuffer=new unsigned char[dataLength];
	in->read(inbuffer, dataLength);
	cout << inbuffer <<endl;
	cout << "cmd:> ";
	bzero(buffer, sizeof(buffer));
    }
}
