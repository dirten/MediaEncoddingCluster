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
	SocketData * data=new SocketData();
	data->data=buffer;
	data->data_length=strlen(buffer);
	cout << "reply:>"<<data->data;
	out->write((const unsigned char*)buffer, strlen(buffer));

//	int writes=mysocket->write(data);
	unsigned char * inbuffer=new unsigned char[1000000];
	in->read(inbuffer, 1000000);
	cout << inbuffer <<endl;
	cout << "cmd:> ";
	bzero(buffer, sizeof(buffer));
	delete data;
    }
}
