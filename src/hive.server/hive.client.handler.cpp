#ifndef HIVE_CLIENT_HANDLER
#define HIVE_CLIENT_HANDLER
#include "org/esb/lang/Runnable.h"
#include "org/esb/net/Socket.h"
#include "hive.frame.reader.cpp"
using namespace org::esb::lang;
using namespace org::esb::net;

class HiveClientHandler: public Runnable{
    private:
	Socket * socket;
//	HiveListener * listener;
	HiveFrameReader * frameReader;
    public:
	HiveClientHandler(Socket * socket){
	    this->socket=socket;
	    this->frameReader=new HiveFrameReader("");
//	    this->listener=listener;
//	    this->listener->addClient(this);
	}
	
	~HiveClientHandler(){
	    cout << "closing HiveClientHandler"<<endl;
//	    this->listener->remove(this);
	    delete socket;
	    socket=0;
	}
	
	void run(){
	    for(int a=0;a<100;a++){

	    Frame * frame=frameReader->getNextFrame();
//	    cout << "sizeof(frame*):"<<sizeof((Frame*)frame)<<endl;
	    socket->getOutputStream()->write((char*)frame->data, frame->length);
	    delete frame;
	    int size=socket->getInputStream()->available(true);
	    unsigned char * buffer=new unsigned char[size];
	    socket->getInputStream()->read(buffer, size);
	    delete buffer;
	    }
	}
};
#endif

