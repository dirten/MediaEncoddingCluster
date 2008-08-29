#include "WebApp.h"
#include <Wt/WContainerWidget>
#include <Wt/WStackedWidget>
#include <Wt/WWidget>
#include <Wt/WText>
#include <Wt/WMenu>
#include <Wt/WMenuItem>
#include <Dashboard.cpp>
#include <Upload.cpp>
#include <Files.cpp>
#include <Profiles.cpp>
#include <Configuration.cpp>

#include <Wt/Ext/Menu>
#include <Wt/Ext/MenuItem>
#include <Wt/Ext/ToolBar>

#include <boost/thread.hpp>
#include <boost/bind.hpp>

namespace org{
namespace esb{
namespace web{

/* Shortcut for a <div id=""> */
class Div : public Wt::WContainerWidget
{
public:
  Div(WContainerWidget *parent, const std::string& id)
    : WContainerWidget(parent)
  {
    setId(id);
  }
};


template <typename Function>
class DeferredWidget : public Wt::WContainerWidget
{
public:
  DeferredWidget(Function f)
    : f_(f) { }

private:
  void load() {
    addWidget(f_());
  }

  Function f_;
};

template <typename Function>
DeferredWidget<Function> *deferCreate(Function f)
{
  return new DeferredWidget<Function>(f);
}

/*
class Dashboard{
  public:
    Wt::WWidget * home(){
      return new Wt::WText("Dashboard Home");
    }
};
*/
WebApp::WebApp(const Wt::WEnvironment & env):WApplication(env){
  setTitle("Hive Webadmin");

  Wt::Ext::Menu * files=new Wt::Ext::Menu();
  Wt::Ext::Menu * profiles=new Wt::Ext::Menu();
  Wt::Ext::Menu * system=new Wt::Ext::Menu();
  Wt::Ext::Menu * help=new Wt::Ext::Menu();
  Wt::Ext::MenuItem * item;

  files->addItem("Files",SLOT(this,WebApp::openFileList));
  profiles->addItem("Profiles",SLOT(this,WebApp::openProfileList));

  system->addItem("Config",this,&WebApp::openConfig);

  Wt::Ext::ToolBar * tb=new Wt::Ext::ToolBar();
  tb->addButton("File", files);
  tb->addButton("Profiles", profiles);
  tb->addButton("System", system);
  tb->addSeparator();
  tb->addButton("?", help);


  contents = new Wt::WStackedWidget();
  contents->setId("main_page");

  Dashboard dashboard;
  Upload upload;


//  Files *files=new Files();

  contents->addWidget(new Files());
  contents->addWidget(new Profiles());
  contents->addWidget(new Configuration());


/*
  Wt::WMenu *menu = new Wt::WMenu(contents, Wt::Vertical, root());
  menu->setRenderAsList(true);
  menu->enableBrowserHistory("main");
  Dashboard dashboard;
  Upload upload;
  Files *files=new Files();
  Profiles *profiles=new Profiles();
  Configuration * config=new Configuration();
  menu->addItem("Dashboard", deferCreate(boost::bind(&Dashboard::home, dashboard)));
  menu->addItem("Upload", deferCreate(boost::bind(&Upload::home, upload)));
  menu->addItem("Files", deferCreate(boost::bind(&Files::home, files)));
  menu->addItem("Profiles", deferCreate(boost::bind(&Profiles::home, profiles)));
  menu->addItem("Configuration", config);
//  files->reload();
  menu->select(0);
*/
  root()->addWidget(tb);
  root()->addWidget(contents);
//  boost::thread t(boost::bind(&Files::removeLastTest, files));
  useStyleSheet("ext/resources/css/xtheme-gray.css");
  
}

void WebApp::openFileList(){
    contents->setCurrentIndex(0);
}

void WebApp::openProfileList(){
    contents->setCurrentIndex(1);
}

void WebApp::openConfig(){
    contents->setCurrentIndex(2);
}

Wt::WWidget *WebApp::introduction()
{
  return new Wt::WText("test Text");
}


Wt::WWidget *WebApp::test()
{
  return new Wt::WText("test Text nummer 2");
}
Wt::WWidget *WebApp::submenu1()
{

  Wt::WStackedWidget *contents = new Wt::WStackedWidget();
  contents->setId("sub_page");

  Wt::WMenu *menu = new Wt::WMenu(contents, Wt::Vertical, root());
  menu->setRenderAsList(true);
//  menu->enableBrowserHistory("main");

  menu->addItem("Introduction 2", introduction());
  menu->addItem("Test Menu 2", test());
  root()->addWidget(contents);

  return menu;
}

}}}

