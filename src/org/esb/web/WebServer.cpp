#include "WebServer.h"
#include "tnt/tntnet.h"
#include <Wt/WApplication>
#include <Wt/WEnvironment>


#include <Wt/WContainerWidget>
#include <Wt/WLineEdit>
#include <Wt/WPushButton>
#include <Wt/WText>
#include <Wt/WBreak>

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

//  b->clicked.connect(SLOT(this, HelloApplication::greet));
//  nameEdit_->enterPressed.connect(SLOT(this, HelloApplication::greet));
}

void MyApplication::greet()
{
  /*
   * Update the text, using text input into the nameEdit_ field.
   */
//  greeting_->setText("Hello there, " + nameEdit_->text());
}




WebServer::WebServer(){


}

WApplication *createApp(const WEnvironment& env)
{
  /*
   * You could read information from the environment to decide whether
   * the user has permission to start a new application
   */
  return new MyApplication(env);
}

void WebServer::run(){
  char * args[]={"test","--docroot",".","--http-address", "0.0.0.0","--http-port", "7070"};
  WRun(7,args,&createApp);
}

void WebServer::run2(){
    tnt::Tntnet app;
    app.listen("0.0.0.0",8008);
    app.mapUrl("^/$","mainpage").setPathInfo("/mainpage");
    app.mapUrl("^/(.*)$","$1");
    app.mapUrl("^/(.*).png$","$1");
    app.run();
}

