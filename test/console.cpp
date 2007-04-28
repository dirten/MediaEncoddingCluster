#include <iostream>
#include "org/esb/net/Socket.h"
#include <signal.h>



using namespace std;
using namespace org::esb::net;
int maxline=1000;
Socket * mysocket;

void catcher(int sig){
    cout << "shutdown" << endl;
    int writes=mysocket->write((unsigned char*)"disconnect",10);
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
    char buffer[maxline];
    cout << "cmd:> ";
    for(;fgets(buffer, maxline, stdin);){
	SocketData * data=new SocketData();
	data->data=buffer;
	data->data_length=strlen(buffer);
	cout << "reply:>"<<buffer;
	int writes=mysocket->write(data);
	data=mysocket->read();
	cout << data->data <<endl;
	cout << "cmd:> ";
	bzero(buffer, sizeof(buffer));
	delete data;
    }
}
