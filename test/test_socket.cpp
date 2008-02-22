#include "org/esb/lang/Runnable.h"
#include "org/esb/lang/Thread.h"
#include "org/esb/net/ServerSocket.h"
#include "org/esb/net/Socket.h"
#include <iostream>
#include <string>
using namespace std;
using namespace org::esb::net;
//using namespace org::esb::av;
using namespace org::esb::io;
class Server:public Runnable{
    public:
	void run(){
	    pserver=new ServerSocket(2000);
	    pserver->bind();
	    if((client=pserver->accept())){
//		int len=client->getInputStream()->available(true);
//		cout << "Erwarte länge von:"<<len<<endl;
//		char *buffer=new char[len];
		std::string str;
		int read=client->getInputStream()->read(str);
//		int read=client->getInputStream()->read((unsigned char*)buffer,len);
		cout << read <<" bytes gelesen"<<endl;
//		delete buffer;
	    }

	}
	~Server(){
	    cout << "Closing Server"<< endl;
	    pserver->close();
		delete pserver;
		client->close();
		delete client;
	}
    private:
	ServerSocket * pserver;
    Socket * client;
};

int main(int argc, char**argv){
    Thread *serverThread=new Thread(new Server());
    serverThread->start();
    Thread::sleep(1000);


    Socket * socket=new Socket("10.122.6.40", 2000);
    socket->connect();
    int len=100000000;
    char *buffer=new char[len];
    memset(buffer,0,len);
    socket->getOutputStream()->write((char*)buffer,len);
    socket->close();
    delete buffer;
    Thread::sleep(1000);
//    socket->close();


//    Thread::sleep(5000);
//    Thread::sleep(1000);

	delete socket;
//    delete server;
    delete serverThread;
}



