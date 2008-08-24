#include "HiveListener.h"
#include "ProtocolServer.h"
#include "org/esb/config/config.h"
#include "org/esb/net/Socket.h"
#include "org/esb/net/ServerSocket.h"
#include "org/esb/lang/Thread.h"
#include "org/esb/signal/Messenger.h"
#include <boost/thread.hpp>
#include <boost/bind.hpp>
//#include "org/esb/lang/Runnable.h"
#include <list>
using namespace org::esb::config;
using namespace org::esb::lang;
using namespace org::esb::net;

namespace org{
namespace esb{
namespace hive{
HiveListener::HiveListener(){
  cout << "new HiveListener"<<endl;
  main_nextloop=true;
  is_running=false;
//  org::esb::signal::Messenger::getInstance().addMessageListener(*this);
}

HiveListener::~HiveListener(){
  cout << "Shutdown HiveListener"<<endl;
}

void HiveListener::run(){
  startListener();
}

void HiveListener::onMessage(org::esb::signal::Message & msg){
  if(msg.getProperty("hivelistener")=="start"){
    cout << "Start Message Arrived:"<<endl;
    boost::thread tt(boost::bind(&HiveListener::startListener,this));
    cout << "Listener running:"<<endl;
    is_running=true;
  }else
  if(msg.getProperty("hivelistener")=="stop"){
    cout << "Stop Message Arrived:"<<endl;
  }
}

void HiveListener::startListener(){
  int port=atoi(Config::getProperty("hive.port"));
    ServerSocket * server=new ServerSocket(port);
    server->bind();
    for(;main_nextloop;){
		try{
	    	Socket * clientSocket=server->accept();
	    	if(clientSocket!=NULL){
	    		ProtocolServer *protoServer=new ProtocolServer(clientSocket);
	    		Thread *thread=new Thread(protoServer);
	    		thread->start();
	    	}else{
				cout << "Client  Socket ist null"<<endl;
				break;
	    	}
		}catch(exception & ex){
	    	cout << "Exception in Main:"<<ex.what();
		}
    }
  }

}}}

