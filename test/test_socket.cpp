#include "org/esb/lang/Runnable.h"
#include "org/esb/lang/Thread.h"
#include "org/esb/net/ServerSocket.h"
#include <sys/ioctl.h>


class Server:public Runnable{
    public:
	void run(){
	    server=new ServerSocket(2000);
	    server->bind();
	    Socket * client;
	    while((client=server->accept())){
		cout << "Client arrived"<< endl;
		while(!client->isClosed()){
		    int dataLength=client->getInputStream()->available(true);
		    cout << "dataLength"<< dataLength<<endl;
		    unsigned char * byte=new unsigned char [dataLength];
		    cout <<"Bytes Readed:"<< client->getInputStream()->read(byte, dataLength);
		    for(int a=0;a<dataLength;a++){
			cout<< "Byte:" << (byte+a)<<endl;
		    }
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
    char * data=(char *)"test\0String";
    socket->getOutputStream()->write(data, 11);
    data=(char *)"test";
    socket->getOutputStream()->write(data, 4);
    data=(char *)"kljfgsjkdfgjsdlöjkfgsldjflgjsdljkfglskdflgjsldkjfglskdjfkgsldkfgsdjfsgjdljfgsdkjgdfs";
    socket->getOutputStream()->write(data, strlen(data));
    data=(char *)"test";
    socket->getOutputStream()->write(data, 4);
//    cout << "\nSenderLength:"<<data<<endl;


    Thread::sleep(1000);
    socket->close();
//    Thread::sleep(1000);


    delete server;
    delete serverThread;
}



