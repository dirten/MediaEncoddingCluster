#include <iostream>
#include "org/esb/net/Socket.h"




using namespace std;
using namespace org::esb::net;
int maxline=1000;

int main(int argc,char**argv){
    Socket * socket=new Socket("localhost", 20000);
    socket->connect();
    char buffer[maxline];
    cout << "cmd:> ";
    for(;fgets(buffer, maxline, stdin);){
	SocketData * data=new SocketData();
	data->data=buffer;
	data->data_length=strlen(buffer);
	int writes=socket->write(data);
	data=socket->read();
	cout << data->data <<endl;
	cout << "cmd:> ";
	delete data;
    }
}
