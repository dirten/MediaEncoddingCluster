#include "org/esb/config/config.h"
#include "org/esb/net/ServerSocket.h"
#include "org/esb/lang/Thread.h"
#include "org/esb/lang/Runnable.h"
#include "hive.client.handler.cpp"

using namespace org::esb::config;
using namespace org::esb::lang;
using namespace org::esb::net;

class HiveListener:public Runnable{
    private:
	int listenerPort;
	ServerSocket * server;
    public:
	HiveListener(){
	    listenerPort=atoi(Config::getProperty("hive.listener.port"));
	}

	~HiveListener(){
	    cout << "Shutdown HiveListener"<<endl;
	    server->close();
	    delete server;
	    server=0;
	}

	void run(){
	    startListener();
	}

	void startListener(){
	    server=new ServerSocket(listenerPort);
	    server->bind();
	    for(;Socket * clientSocket=server->accept();){
		HiveClientHandler * client=new HiveClientHandler(clientSocket);
		Thread thread(client);
		thread.start();
	    }
	}
};

