#include "org/esb/lang/Runnable.h"
#include "org/esb/lang/Thread.h"
#include "org/esb/net/ServerSocket.h"
#include <sys/ioctl.h>
#include <iostream>
#include <fstream>
using namespace std;
class Server:public Runnable{
    public:
	void run(){
	    server=new ServerSocket(2000);
	    server->bind();
	    Socket * client;
	    while((client=server->accept())){
		cout << "Client arrived"<< endl;
		int counter=0;
		while(!client->isClosed()){
//		    Thread::sleep(10000);
		    int dataLength=client->getInputStream()->available(true);
		    cout << "dataLength"<< dataLength<<endl;
		    unsigned char * byte=new unsigned char [dataLength];
		    cout <<"Bytes Readed:"<< client->getInputStream()->read(byte, dataLength)<<endl;;
		    counter+=dataLength;
		cout <<"Insgesam Empfangen"<<counter<<endl;
		    /*
		    for(int a=0;a<dataLength;a++){
			cout<< "Byte:" << (byte+a)<<endl;
		    }
		    */
		    delete byte;
		}
		    
	    }
	    Thread::sleep(1000);

	}
	~Server(){
	    cout << "Closing Server"<< endl;
	    server->close();
	}
    private:
	ServerSocket * server;
};

int main(int argc, char**argv){
    Server * server=new Server();
    Thread *serverThread=new Thread(server);
    serverThread->start();
    Thread::sleep(1000);
    Socket * socket=new Socket("localhost", 2000);
    socket->connect();
    Thread::sleep(1000);
    fstream FileBin("/tmp/frame.container/1/frame100.fstream.ppm",ios::in|ios::out|ios::binary);
    FileBin.seekg(0,ios::end);
    unsigned long filesize=streamoff(FileBin.tellg());
    FileBin.seekg(0,ios::beg);
    string strBuffer="";
    char * buffer=new char [filesize];
    FileBin.read(buffer, filesize);
    socket->getOutputStream()->write(buffer, filesize);
    cout << "file is out"<<endl;
    /*
    char * data=(char *)"test\0String";
    socket->getOutputStream()->write(data, 11);
    data=(char *)"test";
    socket->getOutputStream()->write(data, 4);
    data=(char *)"kljfgsjkdfgjsdlöjkfgsldjflgjsdljkfglskdflgjsldkjfglskdjfkgsldkfgsdjfsgjdljfgsdkjgdfs";
    socket->getOutputStream()->write(data, strlen(data));
    data=(char *)"test";
    socket->getOutputStream()->write(data, 4);
    */
//    cout << "\nSenderLength:"<<data<<endl;


    Thread::sleep(5000);
    socket->close();
//    Thread::sleep(1000);


    delete server;
    delete serverThread;
}



