#ifndef HIVE_CLIENT_HANDLER
#define HIVE_CLIENT_HANDLER
#include "org/esb/lang/Runnable.h"
#include "org/esb/net/Socket.h"
#include "org/esb/av/Frame.h"
#include "org/esb/av/FrameInputStream.h"
#include "org/esb/av/FrameOutputStream.h"
#include "hive.frame.reader.cpp"

using namespace org::esb::lang;
using namespace org::esb::net;
using namespace org::esb::av;

class HiveClientHandler: public Runnable{
    private:
	Socket * socket;
//	HiveListener * listener;
	HiveFrameReader * frameReader;
	FrameInputStream * fis;
	FrameOutputStream * fos;
    public:
	HiveClientHandler(Socket * socket){
	    this->socket=socket;
	    this->frameReader=new HiveFrameReader("../Der Blutige Pfad Gottes - German (DVD-Quali).avi");

		fos=new FrameOutputStream(socket->getOutputStream());
		fis=new FrameInputStream(socket->getInputStream());
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
		fos->writeFrame(frame);
	    delete frame;
		frame=fis->readFrame();
	    delete frame;
	    }
	}
};
#endif

