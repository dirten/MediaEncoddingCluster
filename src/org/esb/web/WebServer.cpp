#include "WebServer.h"
#include "org/esb/config/config.h"
#include <Wt/WApplication>
#include <Wt/WEnvironment>


#include <Wt/WContainerWidget>
#include <Wt/WFileUpload>
#include <Wt/WLineEdit>
#include <Wt/WPushButton>
#include <Wt/WText>
#include <Wt/WBreak>


#include <iostream>

#include "WebApp.h"
#include "Setup.h"
#include "org/esb/util/Log.h"
using namespace org::esb::web;
using namespace Wt;




WApplication *createSetup(const WEnvironment& env)
{
    return new Setup(env);
}

WApplication *createApp(const WEnvironment& env)
{
  /*
   * You could read information from the environment to decide whether
   * the user has permission to start a new application
   */
//  return new MyApplication(env);
    return new WebApp(env);
}

WebServer::WebServer():server("test"){
  char * args[]={
  "mhive",
  "--docroot",org::esb::config::Config::getProperty("web.docroot","."),
  "--http-address", "0.0.0.0",
  "--http-port", org::esb::config::Config::getProperty("web.port","8080"),
  "--no-compression",
  "--deploy-path","/"};
  server.setServerConfiguration(10,args,WTHTTP_CONFIGURATION);
  
  server.addEntryPoint(WServer::Application, &createApp);
  server.addEntryPoint(WServer::Application, &createSetup,"setup");
}

WebServer::~WebServer(){
//	std::cout << "~WebServer"<<std::endl;
}

void WebServer::onMessage(Message & msg){
	if(msg.getProperty("webserver")=="start"){
          try{
          if(!server.isRunning())
            server.start();
          }catch(Wt::WServer::Exception & ex){
            logerror(ex.what());
          }
	  logdebug("Webserver Started");
	}else
	if(msg.getProperty("webserver")=="stop"){
          if(server.isRunning())
            server.stop();
	  logdebug("Webserver Stopped");
	}
}
