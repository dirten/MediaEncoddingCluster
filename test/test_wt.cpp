#include <Wt/WServer>
#include "org/esb/config/config.h"
#include "org/esb/lang/Thread.h"

#include <Wt/WApplication>
#include <Wt/WEnvironment>


using namespace Wt;


class MyApplication : public WApplication
{
public:
  MyApplication(const WEnvironment& env);

private:
//  WLineEdit *nameEdit_;
//  WText *greeting_;

//  void greet();
};


MyApplication::MyApplication(const WEnvironment& env)
  : WApplication(env)
{
/*
  setTitle("Hello world");                               // application title

  root()->addWidget(new WText("Your name, please ? "));  // show some text
  nameEdit_ = new WLineEdit(root());                     // allow text input
  nameEdit_->setFocus();                                 // give focus

  WPushButton *b = new WPushButton("Greet me.", root()); // create a button
  b->setMargin(5, WWidget::Left);                        // add 5 pixels margin 

  root()->addWidget(new WBreak());                       // insert a line break

  greeting_ = new WText(root());                         // empty text
*/
  /*
   * Connect signals with slots
   */

//  b->clicked.connect(SLOT(this, MyApplication::greet));
//  nameEdit_->enterPressed.connect(SLOT(this, MyApplication::greet));
}



WApplication *createApp(const WEnvironment& env)
{
  /*
   * You could read information from the environment to decide whether
   * the user has permission to start a new application
   */
  return new MyApplication(env);
}


int main(){
  char * args[]={
  "test",
  "--docroot",".",
  "--http-address", "0.0.0.0",
  "--http-port", "8081"};
	WRun(7, args, &createApp);
	/*
	Wt::WServer server("test");
	
	server.setServerConfiguration(7,args,WTHTTP_CONFIGURATION);
	server.addEntryPoint(WServer::Application, createApp);
	server.start();
	Thread::sleep(10000);
	server.stop();
	*/
}