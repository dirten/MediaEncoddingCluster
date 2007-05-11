#include <iostream>
#include "org/esb/net/ServerSocket.h"
#include "org/esb/lang/Thread.h"
#include "org/esb/config/config.h"
#include "proto_dispatcher.h"
using namespace std;
using namespace org::esb::net;
using namespace org::esb::config;


int main(int argc,char**argv){
    Config::init("./cluster.cfg");
    int port=atoi(Config::getProperty("listener.port"));
    ServerSocket *server=new ServerSocket(port);
    server->bind();
    for(;Socket * clientSocket=server->accept();){
	Thread * thread=new Thread(new ProtocolServer(clientSocket));
	thread->start();
    }
}
