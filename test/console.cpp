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
    mysocket->getOutputStream()->write((char*)"disconnect\n",12);
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
    bzero(&buffer, maxline);
    cout << "cmd:> ";

    for(;fgets(buffer, maxline, stdin);){
	cout << "reply:>"<<buffer<<":"<<strlen(buffer)<<endl;
	out->write((char*)buffer, strlen(buffer));
	bzero(&buffer, sizeof(buffer));

	int dataLength=in->available(true);
	unsigned char inbuffer[dataLength+1];
	bzero(&inbuffer, dataLength+1);
	in->read(inbuffer, dataLength);
	cout << inbuffer <<endl;
	cout << "cmd:> ";
    }
}
