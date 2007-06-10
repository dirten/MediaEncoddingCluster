#include "org/esb/lang/Runnable.h"
#include "org/esb/lang/Thread.h"
#include "org/esb/io/FileInputStream.h"
#include "org/esb/io/FileOutputStream.h"
#include "org/esb/av/FrameInputStream.h"
#include "org/esb/av/FrameOutputStream.h"
#include "org/esb/av/Frame.h"
#include "org/esb/av/FrameOutputStream.h"
#include "org/esb/net/ServerSocket.h"
#include "org/esb/net/Socket.h"
#include <sys/ioctl.h>
#include <iostream>
#include <fstream>
using namespace std;
using namespace org::esb::net;
using namespace org::esb::av;
using namespace org::esb::io;
class Server:public Runnable{
    public:
	void run(){
	    pserver=new ServerSocket(2000);
	    pserver->bind();
	    while((client=pserver->accept())){
		cout << "Client arrived"<< endl;
//		int counter=0;
		while(!client->isClosed()){
//			cout << "bal"<<endl;


			try{
				FrameInputStream * fis=new FrameInputStream(client->getInputStream());
				Frame * tmp=fis->readFrame();
	
				cout << "FrameArrived"<<endl;
			    FileOutputStream *out2=new FileOutputStream("/tmp/hive/test.1.ppm");
			    FrameOutputStream *fout2=new FrameOutputStream(out2);
			    char header[200];
			    sprintf(header, "P6\n%d %d\n255\n", tmp->getWidth(), tmp->getHeight());
			    fout2->write(header, strlen(header));
			    fout2->writeFrame(tmp);

			    delete fout2;
			    delete out2;
				delete tmp;
				delete fis;
			}catch(Exception &ex){
				ex.printStackTrace();
			}
			/*
//		    Thread::sleep(10000);
		    int dataLength=client->getInputStream()->available(true);
		    cout << "dataLength"<< dataLength<<endl;
		    unsigned char * byte=new unsigned char [dataLength];
		    cout <<"Bytes Readed:"<< client->getInputStream()->read(byte, dataLength)<<endl;;
		    counter+=dataLength;
		    cout <<"Insgesamt Empfangen"<<counter<<endl;
		    //cout <<"Daten"<<byte<<":"<<endl;
		     */
		    /*
		    for(int a=0;a<dataLength;a++){
			cout<< "Byte:" << (byte+a)<<endl;
		    }
		    */
//		    delete byte;
		}
		    
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


    Socket * socket=new Socket("localhost", 2000);
    socket->connect();

    FileInputStream * is =new FileInputStream("/tmp/hive/test.0.raw");
    FrameInputStream * fris=new FrameInputStream(is);
    Frame * fr=fris->readFrame();

	FrameOutputStream * fos=new FrameOutputStream(socket->getOutputStream());
	fos->writeFrame(fr);
	fos->writeFrame(fr);
	fos->writeFrame(fr);

    Thread::sleep(5000);
    socket->close();

	delete fr;
	delete fos;
	delete fris;
	delete is;

    Thread::sleep(5000);
//    Thread::sleep(1000);

	delete socket;
//    delete server;
    delete serverThread;
}



