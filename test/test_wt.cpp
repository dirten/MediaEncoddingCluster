#include <Wt/WServer>
#include "org/esb/config/config.h"
#include "org/esb/lang/Thread.h"

#include <Wt/WApplication>
#include <Wt/WEnvironment>

#include "org/esb/lang/Thread.h"



#include "org/esb/config/config.h"
#include "org/esb/signal/Messenger.h"
#include "org/esb/signal/Message.h"
#include "org/esb/web/WebServer.h"

using namespace Wt;




using namespace org::esb::signal;
using namespace org::esb::web;
using namespace org::esb::config;
int main(){
	Config::init("cluster.cfg");
	WebServer server;

	Messenger::getInstance().addMessageListener(server);
	Messenger::getInstance().sendMessage(Message().setProperty("webserver","start"));


	Thread::sleep(10000);	
//	Messenger::getInstance().sendMessage(Message().setProperty("webserver","stop"));
	Thread::sleep(3000);	

	Config::close();
	Messenger::free();
}