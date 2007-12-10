#include "org/esb/net/ServerSocket.h"
#include "org/esb/net/Socket.h"
#include "org/esb/lang/Thread.h"
#include "org/esb/hive/ProtocolServer.h"
#include "org/esb/config/config.h"
#include "Environment.cpp"
#include <unistd.h>

using namespace org::esb::net;
using namespace org::esb::config;
using namespace org::esb::hive;
int main(int argc, char*argv[]){
//memset(argv[0],0,strlen(argv[0]));
//memcpy(argv[0],(char*)"test",4);

//setproctitle("%s", "bla");
    Config::init("./cluster.cfg");
    if(!checkEnvironment()){
    	cout << "Fehler in der Configuration"<<endl;
    	exit(1);
    }
    
        
    int port=atoi(Config::getProperty("protocol.listener.port"));
    ServerSocket * server=new ServerSocket(port);
    server->bind();



	pthread_mutex_t mutex;
	pthread_mutexattr_t attr;
	pthread_mutexattr_init(&attr);
	pthread_mutex_init(&mutex, &attr);
	pthread_mutexattr_destroy(&attr);

    for(;Socket * clientSocket=server->accept();){
		if(clientSocket!=NULL){
	    	ProtocolServer *protoServer=new ProtocolServer(clientSocket, &mutex);
	    	Thread thread(protoServer);
	    	thread.start();
	    }
    }
}


