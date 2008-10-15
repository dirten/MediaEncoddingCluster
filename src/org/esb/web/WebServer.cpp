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
#include "org/esb/util/Log.h"
using namespace org::esb::web;
using namespace Wt;


class MyApplication : public WApplication
{
public:
  MyApplication(const WEnvironment& env);
  
private:
  WLineEdit *nameEdit_;
  WText *greeting_;
  WFileUpload * upload;
  void greet();
  void uploaded();
  void fileTooLarge();
  void uploadDone();
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


  upload=new WFileUpload(root());

  // Try to catch the fileupload change signal to trigger an upload.
  // We could do like google and at a delay with a WTimer as well...
  upload->changed.connect(SLOT(upload, WFileUpload::upload));

  // React to a succesfull upload.
  upload->uploaded.connect(SLOT(this, MyApplication::uploaded));

  // React to a fileupload problem.
  upload->fileTooLarge.connect(SLOT(this, MyApplication::fileTooLarge));

  /*
   * Connect the uploadDone signal to the Composer's attachmentDone,
   * so that the Composer can keep track of attachment upload progress,
   * if it wishes.
   */
//  uploadDone.connect(SLOT(this, MyApplication::uploadDone));

  /*
   * Connect signals with slots
   */

  b->clicked.connect(SLOT(this, MyApplication::greet));
  nameEdit_->enterPressed.connect(SLOT(this, MyApplication::greet));
}

  void MyApplication::uploaded(){
    std::cout << upload->spoolFileName()<<std::endl;
  }
  void MyApplication::fileTooLarge(){}
  void MyApplication::uploadDone(){}

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
//  return new MyApplication(env);
  return new WebApp(env);
}

WebServer::WebServer():server("test"){
  char * args[]={
  "test",
  "--docroot",org::esb::config::Config::getProperty("web.docroot","."),
  "--http-address", "0.0.0.0",
  "--http-port", org::esb::config::Config::getProperty("web.port","8080"),
  "--no-compression"};
    logdebug("!!!!!!!!!!!!!!!!Webserver Root:"<<org::esb::config::Config::getProperty("web.docroot","."));
	server.setServerConfiguration(8,args,WTHTTP_CONFIGURATION);
	server.addEntryPoint(WServer::Application, &createApp);
}

WebServer::~WebServer(){
//	std::cout << "~WebServer"<<std::endl;
}

void WebServer::onMessage(Message & msg){
	if(msg.getProperty("webserver")=="start"){
	  server.start();
	  logdebug("Webserver Started");
	}else
	if(msg.getProperty("webserver")=="stop"){
	  server.stop();
	  logdebug("Webserver Stopped");
	}
}
