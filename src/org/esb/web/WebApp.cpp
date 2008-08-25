#include "WebApp.h"
#include <Wt/WContainerWidget>
#include <Wt/WStackedWidget>
#include <Wt/WWidget>
#include <Wt/WText>
#include <Wt/WMenu>
#include <Wt/WMenuItem>
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


WebApp::WebApp(const Wt::WEnvironment & env):WApplication(env){
  setTitle("test");

  Wt::WStackedWidget *contents = new Wt::WStackedWidget();
  contents->setId("main_page");

  Wt::WMenu *menu = new Wt::WMenu(contents, Wt::Vertical, root());
  menu->setRenderAsList(true);
  menu->enableBrowserHistory("main");

  menu->addItem("Introduction", introduction());
  menu->addItem("Test Menu", test());
//  menu->addItem("Test Sub Menu", submenu1());
  menu->addItem(new Wt::WMenuItem("Demo2", submenu1(),Wt::WMenuItem::LazyLoading));
  menu->select(1);

  root()->addWidget(contents);
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

