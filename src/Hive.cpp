//#include <iostream>
//#include <stdlib.h>
#include <boost/program_options.hpp>
#include "org/esb/signal/Message.h"
#include "org/esb/signal/Messenger.h"

#include "org/esb/net/TcpServerSocket.h"
#include "org/esb/net/TcpSocket.h"
#include "org/esb/lang/Thread.h"
//#include "org/esb/hive/ProtocolServer.h"
#include "org/esb/config/config.h"
//#include "org/esb/hive/job/JobWatcher.h"
#include "org/esb/hive/job/ProcessUnitWatcher.h"
#include "org/esb/hive/job/ProcessUnit.h"
#include "org/esb/io/ObjectInputStream.h"
#include "org/esb/io/ObjectOutputStream.h"
#include "org/esb/web/WebServer.h"
#include "org/esb/hive/HiveListener.h"
#include "org/esb/hive/DirectoryScanner.h"
#include "org/esb/av/AV.h"
//#include "org/esb/hive/PacketCollector.h"
#include "Environment.cpp"



#include "org/esb/util/Decimal.h"
//#include "org/esb/hive/FileImporter.h"
//#include "export.cpp"
//#include "org/esb/util/Log.h"
//#include "job.cpp"
//#include "org/esb/hive/JobUtil.h"
#if !defined(_WIN32)

#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>

#endif  // !_WIN32

using namespace org::esb::net;
using namespace org::esb::io;
using namespace org::esb::config;
using namespace org::esb::hive;
using namespace org::esb::hive::job;
using namespace org::esb::web;
using namespace org::esb::util;
using namespace org::esb::signal;
using namespace std;



using namespace std;

namespace po = boost::program_options;

void listener(int argc, char * argv[]);
void client(int argc, char * argv[]);
void shell(int argc, char * argv[]);


int main(int argc, char * argv[]){	
//    loginit("log.properties");
    std::string config_path;
    po::options_description gen("general options");
    gen.add_options()
        ("help", "produce this message")
        ("config,c", po::value<std::string>()->default_value("/etc/hive.conf"), "use Configuration")
        ("version,v", "Prints the Version")
        ;

    po::options_description ser("Server options");
    ser.add_options()
        ("server,s", "start the Hive Server Process")
        ("port,p", po::value<int>()->default_value(20200), "specify the port for the Hive Server")
        ("web,w", po::value<int>()->default_value(8080), "start the Web Server Process on the specified port")
        ("webroot,r", po::value<std::string>()->default_value("."), "define the Path for Web Server root")
        ("scandir", po::value<std::string>()->default_value("."), "define the Path to Scan for new Media Files")
        ;


    po::options_description cli("Client options");
    cli.add_options()
        ("client,i","start the Hive Client")
        ("host,h", po::value<std::string>()->default_value("localhost"), "Host to connect")
        ("port,p", po::value<int>()->default_value(20200), "Port to connect")
        ;

    po::options_description all("options");
    all.add(gen).add(ser).add(cli);

    gen.add(ser).add(cli);

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, gen), vm);
    po::notify(vm);
    
    if (argc==1 || vm.count("help")) {
      cout << gen << "\n";
      return 1;
    }

	av_register_all ();
    avcodec_init();
	avcodec_register_all ();
	
    Config::init((char*)vm["config"].as<std::string>().c_str());
	

    if (vm.count("server")) {
//      logdebug("setting webroot to :"<<vm["webroot"].as<std::string>());
  	  Config::setProperty("web.docroot",vm["webroot"].as<std::string>().c_str());
  	  if(vm.count("scandir"))
		Config::setProperty("hive.scandir",vm["scandir"].as<std::string>().c_str());
  	  Config::setProperty("hive.port",Decimal(vm["port"].as<int>()).toString().c_str());
  	  listener(argc, argv);
    }

    if (vm.count("client")) {
  	  Config::setProperty("client.port",Decimal(vm["port"].as<int>()).toString().c_str());
  	  Config::setProperty("client.host",vm["host"].as<std::string>().c_str());
	  client(argc,argv);
    }


//    Config::init("./cluster.cfg");
/*
    loginit(Config::getProperty("log.conf"));

	av_register_all ();
    avcodec_init();
	avcodec_register_all ();
*/
/*
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
//			cout << "Argument unknown : "<<argv[arg_counter]<<endl;
			exit(-127);
		}		
	}
	*/
	Config::close();
	return 0;
}

void client(int argc, char *argv[]){

	string host=Config::getProperty("client.host");
	int port=atoi(Config::getProperty("client.port"));

  	cout << "Connecting to "<<host<<" on port "<<port<<endl;;
    TcpSocket sock((char*)host.c_str(), port);
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
//		try{
			unit.process();
//		}catch(...){
//			logerror("Error in process");
//		}
		char * text_out="put process_unit";
		sock.getOutputStream()->write(text_out, strlen(text_out));
		oos.writeObject(unit);
//		break;
    }
//    break;
	org::esb::lang::Thread::sleep2(1000);
}
}

/*----------------------------------------------------------------------------------------------*/
bool main_nextLoop=true;


#ifdef WIN32

boost::mutex     terminationMutex;
boost::condition ctrlCHit;
boost::condition serverStopped;

BOOL WINAPI console_ctrl_handler(DWORD ctrl_type)
{
  switch (ctrl_type)
  {
  case CTRL_C_EVENT:
  case CTRL_BREAK_EVENT:
  case CTRL_CLOSE_EVENT:
  case CTRL_SHUTDOWN_EVENT:
    {
      boost::mutex::scoped_lock terminationLock(terminationMutex);

      ctrlCHit.notify_all(); // should be just 1

      serverStopped.wait(terminationLock);
      return TRUE;
    }
  default:
    return FALSE;
  }
}


void ctrlCHitWait(){
    boost::mutex::scoped_lock terminationLock(terminationMutex);
    ctrlCHit.wait(terminationLock);
}

#else

void ctrlCHitWait(){
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

}

#endif

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

	WebServer webserver;
   	Messenger::getInstance().addMessageListener(webserver);

	HiveListener hive;
   	Messenger::getInstance().addMessageListener(hive);

    DirectoryScanner dirscan(Config::getProperty("hive.scandir"),atoi(Config::getProperty("hive.scaninterval", "300"))*1000);
   	Messenger::getInstance().addMessageListener(dirscan);

	ProcessUnitWatcher puw;
   	Messenger::getInstance().addMessageListener(puw);

//    JobWatcher watcher(*JobHandler::getInstance());
//   	Messenger::getInstance().addMessageListener(watcher);
   	
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
   		Messenger::getInstance().sendRequest(Message().setProperty("webserver","start"));

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
      Thread::sleep2(10000);
    }
*/

    ctrlCHitWait();

	Messenger::getInstance().sendMessage(Message().setProperty("directoryscan","stop"));    
	Messenger::getInstance().sendMessage(Message().setProperty("jobwatcher","stop"));
	Messenger::getInstance().sendMessage(Message().setProperty("processunitwatcher","stop"));
	Messenger::getInstance().sendMessage(Message().setProperty("hivelistener","stop"));
	Messenger::getInstance().sendMessage(Message().setProperty("webserver","stop"));
//    Config::close();
	Messenger::free();


	org::esb::lang::Thread::sleep2(3000);
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

