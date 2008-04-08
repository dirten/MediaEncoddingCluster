#include <iostream>


#include "org/esb/net/ServerSocket.h"
#include "org/esb/net/Socket.h"
#include "org/esb/lang/Thread.h"
#include "org/esb/hive/ProtocolServer.h"
#include "org/esb/config/config.h"
#include "org/esb/hive/job/JobWatcher.h"
#include "org/esb/io/ObjectInputStream.h"
#include "org/esb/io/ObjectOutputStream.h"

#include "Environment.cpp"
#include <unistd.h>
#include <stdlib.h>

#include "import.cpp"
#include "export.cpp"
//#include "job.cpp"
using namespace org::esb::net;
using namespace org::esb::config;
using namespace org::esb::hive;
using namespace std;


using namespace std;


void listener(int argc, char * argv[]);
void client(int argc, char * argv[]);
void shell(int argc, char * argv[]);



int main(int argc, char * argv[]){	

    Config::init("./cluster.cfg");

	for(int arg_counter=1;argc>arg_counter;arg_counter++){
		if(strcmp(argv[arg_counter],"listen")==0){
			listener(argc, argv);
		}else if(strcmp(argv[arg_counter],"shell")==0){
				cout << "shell is not implemeted now!!!"<<endl;
		}else if(strcmp(argv[arg_counter],"client")==0){
			client(argc,argv);
		}else if(strcmp(argv[arg_counter],"job")==0){
//			jobcreator(argc,argv);
		}else if(strcmp(argv[arg_counter],"import")==0){
			import(--argc,++argv);
			return 0;
		}else if(strcmp(argv[arg_counter],"export")==0){
			exporter(argc,argv);
		}else{
			cout << "Argument unknown : "<<argv[arg_counter]<<endl;
			exit(-127);
		}		
	}
	Config::close();
	return 0;
}

void client(int argc, char *argv[]){
    Socket sock("localhost", 20000);
    sock.connect();
    ObjectInputStream ois(sock.getInputStream());
    ObjectOutputStream oos(sock.getOutputStream());
    int pCount=0;
    while(true||++pCount<20){
		char * text="get process_unit";
		sock.getOutputStream()->write(text, strlen(text));
		ProcessUnit unit;
		ois.readObject(unit);
		if(unit._input_packets.size()==0)break;
		unit.process();
		char * text_out="put process_unit";
		sock.getOutputStream()->write(text_out, strlen(text_out));
		oos.writeObject(unit);
    }
}

/*----------------------------------------------------------------------------------------------*/
bool main_nextLoop=true;
void listener(int argc, char *argv[]){
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
    for(;main_nextLoop;){
		try{
	    	Socket * clientSocket=server->accept();
	    	if(clientSocket!=NULL){
	    		ProtocolServer *protoServer=new ProtocolServer(clientSocket);
	    		Thread thread(protoServer);
	    		thread.start();
	    	}else{
				cout << "Client  Socket ist null"<<endl;
				break;
	    	}
		}catch(exception & ex){
	    	cout << "Exception in Main:"<<ex.what();
		}
    }
}

