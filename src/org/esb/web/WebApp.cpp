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
//#include <ProfilesEdit.cpp>
#include <Configuration.cpp>

#include <Wt/Ext/Menu>
#include <Wt/Ext/MenuItem>
#include <Wt/Ext/ToolBar>
#include <Wt/Ext/TabWidget>


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
  Wt::WContainerWidget * main=new Wt::WContainerWidget(root());
  main->resize(Wt::WLength(100, Wt::WLength::Percentage), Wt::WLength(100, Wt::WLength::Percentage));

  Wt::Ext::TabWidget * tab=new Wt::Ext::TabWidget(main);
  //tab->resize(Wt::WLength(100, Wt::WLength::Percentage), Wt::WLength(100, Wt::WLength::Percentage));

//  tab->resize(1000,1000);
	sql::Connection con(Config::getProperty("db.connection"));
	sql::Statement stmt=con.createStatement("select id, filename, container_type, size from files ");
	SqlTableModel * model=new SqlTableModel(stmt.executeQuery());
	Wt::Ext::TableView * table;
	tab->addTab(table=new Wt::Ext::TableView(),"Files");
	table->setModel(model);
  tab->resize(1000,500);


//  tab->addTab(new Profiles(),"Profiles");
//  tab->addTab(new Configuration(),"Configuration");

  useStyleSheet("ext/resources/css/xtheme-gray.css");
  useStyleSheet("filetree.css");
  
}

void WebApp::openFileUpload(){
    contents->setCurrentIndex(3);
}
void WebApp::openFileList(){
    contents->setCurrentIndex(0);
}

void WebApp::openProfileNew(){
    contents->setCurrentIndex(4);
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

/*
Wt::WWidget *WebApp::test()
{
  return new Wt::WText("test Text nummer 2");
}
*/
Wt::WWidget *WebApp::submenu1()
{

  Wt::WStackedWidget *contents = new Wt::WStackedWidget();
  contents->setId("sub_page");

  Wt::WMenu *menu = new Wt::WMenu(contents, Wt::Vertical, root());
  menu->setRenderAsList(true);
//  menu->enableBrowserHistory("main");

  menu->addItem("Introduction 2", introduction());
//  menu->addItem("Test Menu 2", test());
  root()->addWidget(contents);

  return menu;
}

}}}

