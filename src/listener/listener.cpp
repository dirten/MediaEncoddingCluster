#include <iostream>
#include "org/esb/net/ServerSocket.h"
#include "org/esb/lang/Thread.h"
#include "proto_dispatcher.h"
using namespace std;
using namespace org::esb::net;


int main(int argc,char**argv){

    ServerSocket *server=new ServerSocket(20000);
    server->bind();
    for(;Socket * clientSocket=server->accept();){
	Thread * thread=new Thread(new ProtocolServer(clientSocket));
	thread->start();
    }
}
