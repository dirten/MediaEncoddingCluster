#include "WebApp2.h"
#include "api/xml/XmlApp.h"
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
#include <Wt/WLogger>


#include <iostream>

//#include "WebApp.h"
#include "Setup.h"
#include "org/esb/util/Log.h"
//#include "TestApp.cpp"
using namespace org::esb::web;
using namespace Wt;

WApplication *createTestApp(const WEnvironment& env) {
  //return new TestApp(env);
  return NULL;
}

WApplication *createSetup(const WEnvironment& env) {
  return new Setup(env);
}

WApplication *createApp(const WEnvironment& env) {
  /*
   * You could read information from the environment to decide whether
   * the user has permission to start a new application
   */
  //  return new MyApplication(env);

  return new WebApp2(env);
}

WebServer::WebServer() : server("test") {
  std::string weblog_file = org::esb::config::Config::get("log.path");
  weblog_file.append("/http.log");
  std::string syslog_file = org::esb::config::Config::get("log.path");
  syslog_file.append("/sys.log");
  std::string doc_root=org::esb::config::Config::get("hive.base_path");
  doc_root+="/web";
  LOGDEBUG("weblog="<<weblog_file);
  LOGDEBUG("syslog="<<syslog_file);
  LOGDEBUG("docroot="<<doc_root);
  const char * args[] = {
    "mhive",
    "--docroot", const_cast<char*> (doc_root.c_str()),
    "--http-address", "0.0.0.0",
    "--http-port", org::esb::config::Config::getProperty("web.port", "8080"),
    "--accesslog", const_cast<char*> (weblog_file.c_str()),
    "--no-compression",
    "--deploy-path", "/"
    //"--max-memory-request-size", "10000000000"
  };
/*
    Wt::WLogger stderrLogger;
    stderrLogger.setFile(syslog_file);
    ostringstream bla;
    stderrLogger.setStream(bla);
*/
  for(int a=0;a<12;a++){
    LOGDEBUG(args[a]);
  }
  //LOGDEBUG("#serverargs:"<<sizeof(args));
  server.setServerConfiguration(12, const_cast<char**>(args), WTHTTP_CONFIGURATION);
//  return;
  try{
    server.addEntryPoint(Application, &createApp, "/");
    server.addEntryPoint(Application, &createSetup, "/setup");
    server.addResource(new XmlApp(),"/api/xml");
    //addEntryPoint(StaticResource, &createXmlApi, "/api/xml");
  }catch(std::exception ex){
    LOGERROR("error configure webserver:"<<ex.what());
  }
  //  server.addEntryPoint(WServer::Application, &createTestApp, "test");
  //  logdebug(typeid(*this).name());
}

WebServer::~WebServer() {
  //	std::cout << "~WebServer"<<std::endl;
}

void WebServer::start() {
  server.start();
  //  WServer::waitForShutdown();
}

void WebServer::run() {
  server.start();
  WServer::waitForShutdown();
}

void WebServer::stop() {
  server.stop();

}

void WebServer::onMessage(Message & msg) {
  if (msg.getProperty("webserver") == "start") {
    try {
      if (!server.isRunning())
        server.start();
      LOGDEBUG("Webserver Started");
    } catch (Wt::WServer::Exception & ex) {
      LOGERROR(ex.what());
    }
  } else
    if (msg.getProperty("webserver") == "stop") {
    if (server.isRunning())
      server.stop();
    LOGDEBUG("Webserver Stopped");
  }
}
extern "C"{
void start_webserver(){
    org::esb::web::WebServer * s=new org::esb::web::WebServer();
    s->run();
}
}