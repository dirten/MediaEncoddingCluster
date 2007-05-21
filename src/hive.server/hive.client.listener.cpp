#ifndef HIVE_CLIENT_LISTENER
#define HIVE_CLIENT_LISTENER
#include "org/esb/config/config.h"
#include "org/esb/net/ServerSocket.h"
#include "org/esb/lang/Thread.h"
#include "org/esb/lang/Runnable.h"
#include "hive.client.handler.cpp"
#include <list>
using namespace org::esb::config;
using namespace org::esb::lang;
using namespace org::esb::net;

class HiveListener:public Runnable{
    private:
	int listenerPort;
	ServerSocket * server;
	list<HiveClientHandler *>_clients;
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
		this->addClient(client);
		Thread thread(client);
		thread.start();
	    }
	}

	void addClient(HiveClientHandler * client){
	    _clients.push_back(client);
	}
	
	void removeClient(HiveClientHandler * client){
	    _clients.remove(client);	
	}

	list<HiveClientHandler *> getClients(){
	    return _clients;
	}
};
#endif

