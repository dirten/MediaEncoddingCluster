#include "WebServer.h"
#include "org/esb/config/config.h"
#include <Wt/WApplication>
#include <Wt/WEnvironment>


#include <Wt/WContainerWidget>
#include <Wt/WLineEdit>
#include <Wt/WPushButton>
#include <Wt/WText>
#include <Wt/WBreak>

#include <boost/thread.hpp>
#include <boost/bind.hpp>

#include <iostream>
using namespace org::esb::web;
using namespace Wt;


class MyApplication : public WApplication
{
public:
  MyApplication(const WEnvironment& env);

private:
  WLineEdit *nameEdit_;
  WText *greeting_;

  void greet();
};


MyApplication::MyApplication(const WEnvironment& env)
  : WApplication(env)
{
  setTitle("Hello world");                               // application title

  root()->addWidget(new WText("Your name, please ? "));  // show some text
  nameEdit_ = new WLineEdit(root());                     // allow text input
  nameEdit_->setFocus();                                 // give focus

  WPushButton *b = new WPushButton("Greet me.", root()); // create a button
  b->setMargin(5, WWidget::Left);                        // add 5 pixels margin 

  root()->addWidget(new WBreak());                       // insert a line break

  greeting_ = new WText(root());                         // empty text

  /*
   * Connect signals with slots
   */

  b->clicked.connect(SLOT(this, MyApplication::greet));
  nameEdit_->enterPressed.connect(SLOT(this, MyApplication::greet));
}

void MyApplication::greet()
{
  /*
   * Update the text, using text input into the nameEdit_ field.
   */
  greeting_->setText("Hello there, " + nameEdit_->text());
}



WApplication *createApp(const WEnvironment& env)
{
  /*
   * You could read information from the environment to decide whether
   * the user has permission to start a new application
   */
  return new MyApplication(env);
}

WebServer::WebServer():server("test"){
  char * args[]={
  "test",
  "--docroot",org::esb::config::Config::getProperty("web.docroot"),
  "--http-address", "0.0.0.0",
  "--http-port", org::esb::config::Config::getProperty("web.port")};


	server.setServerConfiguration(7,args,WTHTTP_CONFIGURATION);
	server.addEntryPoint(WServer::Application, &createApp);
}

WebServer::~WebServer(){
	std::cout << "~WebServer"<<std::endl;

}


void WebServer::run(){
  char * args[]={"test","--docroot",".","--http-address", "0.0.0.0","--http-port", org::esb::config::Config::getProperty("web.port")};
  WRun(7,args,&createApp);
}

void WebServer::onMessage(Message & msg){
	if(msg.getProperty("webserver")=="start"){
//		boost::thread t(boost::bind(&WebServer::start,this));
	server.start();
//		start();
	}else
	if(msg.getProperty("webserver")=="stop"){
	  server.stop();
	}
}
void WebServer::start(){


//  WRun(7,args,&createApp);
}

