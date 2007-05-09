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
		Thread::sleep(1000);
//		cout << "dataLength"<< client->getInputStream()->available()<<endl;
	#if defined(FIONREAD)

        int numBytes = 0;
	    if( ::ioctl (client->getDescriptor(), FIONREAD, &numBytes) != -1 ){
            cout << "FIONREAD"<<numBytes<<endl;
        }
         
	#endif
/*
        std::size_t numBytes = 0;
	    if( ::ioctl (client->getDescriptor(), FIONREAD, &numBytes) != -1 ){
            cout << "FIONREAD"<<numBytes<<endl;
        }
        cout << "NameMto"<<numBytes<<endl;
*/
//		while(client->getInputStream()->available()>0){
//
    /*
		    unsigned char * byte=new unsigned char [10];
		    string buffer;
		    cout <<"Bytes Avialable:"<< client->getInputStream()->available();
		    cout <<"Bytes Readed:"<< client->getInputStream()->read(&buffer);
		    const char * byte=buffer.data();
		    for(int a=0;a<10;a++){
//			write(0,(byte+a),1);
			cout<< "Byte:" << (byte+a)<<endl;
		    }
		    */
//		    delete byte;
//		}
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
    Thread::sleep(500);
    Socket * socket=new Socket("localhost", 2000);
    socket->connect();
    const unsigned char * data=(const unsigned char *)"testString";
//    string tmp="test\0String";

    socket->getOutputStream()->write(data, 10);
//    cout << "\nSenderLength:"<<data<<endl;


    Thread::sleep(1000);
    socket->close();
    Thread::sleep(1000);


    delete server;
    delete serverThread;
}



