#include <iostream>
#include "org/esb/net/ServerSocket.h"
#include "org/esb/lang/Thread.h"
#include "org/esb/config/config.h"
#include "proto_dispatcher.h"
#include <signal.h>
using namespace std;
using namespace org::esb::net;
using namespace org::esb::config;
ServerSocket *server;


void catcher(int sig){
    cout << "shutdown listener" << endl;
//    server->close();
//    delete server;
    exit(0);
}

void setCatcher(){
    signal(SIGABRT, &catcher);
    signal(SIGTERM, &catcher);
    signal(SIGQUIT, &catcher);
    signal(SIGHUP, &catcher);
    signal(SIGINT, &catcher);
}

int main(int argc,char**argv){
    setCatcher();
    Config::init("./cluster.cfg");
    int port=atoi(Config::getProperty("listener.port"));

    server=new ServerSocket(port);
    server->bind();
    for(;Socket * clientSocket=server->accept();){
	ProtocolServer *protoServer=new ProtocolServer(clientSocket);
	Thread thread(protoServer);
	thread.start();
    }
//    delete server;
}
