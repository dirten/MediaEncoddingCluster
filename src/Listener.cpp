#include "org/esb/net/ServerSocket.h"
#include "org/esb/net/Socket.h"
#include "org/esb/lang/Thread.h"
#include "org/esb/hive/ProtocolServer.h"
#include "org/esb/config/config.h"

using namespace org::esb::net;
using namespace org::esb::config;
using namespace org::esb::hive;
int main(){
    Config::init("./cluster.cfg");
    int port=atoi(Config::getProperty("protocol.listener.port"));
    ServerSocket * server=new ServerSocket(port);
    server->bind();
			pthread_mutex_t mutex;
    		pthread_mutexattr_t attr;
			pthread_mutexattr_init(&attr);
			pthread_mutex_init(&mutex, &attr);
			pthread_mutexattr_destroy(&attr);

    for(;Socket * clientSocket=server->accept();){
	    ProtocolServer *protoServer=new ProtocolServer(clientSocket, mutex);
	    Thread thread(protoServer);
	    thread.start();
    }
}


