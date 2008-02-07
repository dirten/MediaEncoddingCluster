#include "org/esb/net/ServerSocket.h"
#include "org/esb/net/Socket.h"
#include "org/esb/lang/Thread.h"
#include "org/esb/hive/ProtocolServer.h"
#include "org/esb/config/config.h"
#include "org/esb/hive/job/JobWatcher.h"
#include "Environment.cpp"
#include <unistd.h>
#include <stdlib.h>
using namespace org::esb::net;
using namespace org::esb::config;
using namespace org::esb::hive;
using namespace std;

bool main_nextLoop=true;
int main(int argc, char*argv[]){
/*
string prog=argv[0];
prog+="test";
//delete [] argv[0];
argv[0]=new char[prog.length()];
memset(argv[0],0,strlen(argv[0]));
memcpy(argv[0],(char*)prog.c_str(),prog.length());


setproctitle("%s", "bla");
*/
    
    Config::init("./cluster.cfg");
    if(!checkEnvironment()){
    	cout << "Fehler in der Configuration"<<endl;
    	exit(1);
    }
    

    JobWatcher *_watcher=new JobWatcher(*JobHandler::getInstance());
    Thread *runner=new Thread(_watcher);
    runner->start();

        
    int port=atoi(Config::getProperty("protocol.listener.port"));
    ServerSocket * server=new ServerSocket(port);
    server->bind();


//    bool nextLoop=true;
    try{
    for(;main_nextLoop;){
	    Socket * clientSocket=server->accept();
		if(clientSocket!=NULL){
	    	ProtocolServer *protoServer=new ProtocolServer(clientSocket);
	    	Thread thread(protoServer);
	    	thread.start();
	    }
    }
    }catch(exception & ex){
	cout << "Exception in Main:"<<ex.what();
    }
}


