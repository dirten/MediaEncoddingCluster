#include <iostream>


#include "org/esb/net/ServerSocket.h"
#include "org/esb/net/Socket.h"
#include "org/esb/lang/Thread.h"
#include "org/esb/hive/ProtocolServer.h"
#include "org/esb/config/config.h"
#include "org/esb/hive/job/JobWatcher.h"
#include "org/esb/hive/job/ProcessUnitWatcher.h"
#include "org/esb/io/ObjectInputStream.h"
#include "org/esb/io/ObjectOutputStream.h"
#include "org/esb/web/WebServer.h"
#include "org/esb/hive/HiveListener.h"
#include "org/esb/hive/DirectoryScanner.h"

#include "org/esb/hive/PacketCollector.h"
#include "Environment.cpp"

#include "org/esb/signal/Message.h"
#include "org/esb/signal/Messenger.h"

#include <unistd.h>
#include <stdlib.h>

#include "org/esb/hive/FileImporter.h"
#include "export.cpp"
#include "org/esb/util/Log.h"
//#include "job.cpp"
#include "org/esb/hive/JobUtil.h"
#if !defined(_WIN32)

#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>

#endif  // !_WIN32

using namespace org::esb::net;
using namespace org::esb::config;
using namespace org::esb::hive;
using namespace org::esb::web;
using namespace org::esb::signal;
using namespace std;



using namespace std;


void listener(int argc, char * argv[]);
void client(int argc, char * argv[]);
void shell(int argc, char * argv[]);



int main(int argc, char * argv[]){	
//    loginit("log.properties");
    Config::init("./cluster.cfg");

    loginit(Config::getProperty("log.conf"));

	for(int arg_counter=1;argc>arg_counter;arg_counter++){
		if(strcmp(argv[arg_counter],"listen")==0){
			listener(argc, argv);
		}else if(strcmp(argv[arg_counter],"shell")==0){
				cout << "shell is not implemeted now!!!"<<endl;
		}else if(strcmp(argv[arg_counter],"client")==0){
			client(argc,argv);
		}else if(strcmp(argv[arg_counter],"job")==0){
			jobcreator(argc,argv);
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
	string host="localhost";
  	if(argc==3){
  		host=argv[2];
  	}
  	cout << "Connecting to "<<host<<endl;;
    Socket sock((char*)host.c_str(), 20000);
    sock.connect();
    ObjectInputStream ois(sock.getInputStream());
    ObjectOutputStream oos(sock.getOutputStream());
    int pCount=0;
while(true){
    while(true||++pCount<20){
		char * text="get process_unit";
		sock.getOutputStream()->write(text, strlen(text));
		ProcessUnit unit;
		ois.readObject(unit);
		if(unit._input_packets.size()==0)break;
		try{
			unit.process();
		}catch(...){
			logerror("Error in process");
		}
		char * text_out="put process_unit";
		sock.getOutputStream()->write(text_out, strlen(text_out));
		oos.writeObject(unit);
//		break;
    }
//    break;
	Thread::sleep(10000);
}
}

/*----------------------------------------------------------------------------------------------*/
bool main_nextLoop=true;

void listener(int argc, char *argv[]){
//    Config::init("./cluster.cfg");
    if(!checkEnvironment()){
    	cout << "Fehler in der Configuration"<<endl;
    	exit(1);
    }
    /*
    *
    * Initializing Application Services
    *
    */

   	HiveListener hive;
   	Messenger::getInstance().addMessageListener(hive);

    DirectoryScanner dirscan(Config::getProperty("hive.scandir"),atoi(Config::getProperty("hive.scaninterval", "300"))*1000);
   	Messenger::getInstance().addMessageListener(dirscan);

	WebServer webserver;
   	Messenger::getInstance().addMessageListener(webserver);

	ProcessUnitWatcher puw;
   	Messenger::getInstance().addMessageListener(puw);

    JobWatcher watcher(*JobHandler::getInstance());
   	Messenger::getInstance().addMessageListener(watcher);
   	
    /*
    *
    * Starting Application Services from configuration
    *
    */

	if(string(Config::getProperty("hive.autoscan"))=="true"){
   		Messenger::getInstance().sendMessage(Message().setProperty("directoryscan","start"));
	}

	if(string(Config::getProperty("hive.start"))=="true"){
//   		Messenger::getInstance().sendMessage(Message().setProperty("processunitwatcher","start"));
   		Messenger::getInstance().sendMessage(Message().setProperty("jobwatcher","start"));
   		Messenger::getInstance().sendMessage(Message().setProperty("hivelistener","start"));
   	}

	if(string(Config::getProperty("web.start"))=="true")
   		Messenger::getInstance().sendMessage(Message().setProperty("webserver","start"));

//	Thread::sleep(5000);
//   		Messenger::getInstance().sendMessage(Message().setProperty("webserver","stop"));

    /*
    * @todo
    * replace Thread with Messaging
    */
/*
    ProcessUnitWatcher *unit_watcher=new ProcessUnitWatcher();
    Thread *unitRunner=new Thread(unit_watcher);
*/
//    JobWatcher *_watcher=new JobWatcher(*JobHandler::getInstance());
//    Thread *runner=new Thread(_watcher);
/*
    PacketCollector *_collector=new PacketCollector();
    Thread *collector_runner=new Thread(_collector);
    collector_runner->start();
  */  
/*
    WebServer * webServer=new WebServer();
    Thread * webRunner=new Thread(webServer);
  */
//    webRunner->start();
//    runner->start();
//    unitRunner->start();

/*
    while(true){
      Thread::sleep(10000);
    }
*/

	sigset_t wait_mask2;
	sigemptyset(&wait_mask2);
	sigaddset(&wait_mask2, SIGINT);
	sigaddset(&wait_mask2, SIGQUIT);
	sigaddset(&wait_mask2, SIGTERM);
	//sigaddset(&wait_mask, SIGCHLD);
	pthread_sigmask(SIG_BLOCK, &wait_mask2, 0);
	int sig = 0;
	//sigdelset(&wait_mask, SIGCHLD);
    
	int err;
	do {
	  err = sigwait(&wait_mask2, &sig);
	} while (err != 0);
	Messenger::getInstance().sendMessage(Message().setProperty("directoryscan","stop"));    
	Messenger::getInstance().sendMessage(Message().setProperty("jobwatcher","stop"));
	Messenger::getInstance().sendMessage(Message().setProperty("processunitwatcher","stop"));
	Messenger::getInstance().sendMessage(Message().setProperty("hivelistener","stop"));
	Messenger::getInstance().sendMessage(Message().setProperty("webserver","stop"));
//    Config::close();
	Messenger::free();


	Thread::sleep(3000);
	cout << "Stopping Hive "<<endl;

/*
    while(true){
      Thread::sleep(10000);
    }
*/
//    hive.run();
/*    
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
  */
}

