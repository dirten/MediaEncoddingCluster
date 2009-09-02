#include <Wt/WEnvironment>
#include <Wt/WApplication>
#include <Wt/WContainerWidget>

#include <Wt/Ext/Container>
#include <Wt/Ext/Panel>
#include <Wt/WLayout>
#include <Wt/WFitLayout>
#include <Wt/WVBoxLayout>
#include <Wt/WBoxLayout>
#include <Wt/WTimer>
#include <Wt/WBorderLayout>
//#include <Wt/Ext/BorderLayoutImpl.h>
#include "SqlTable.h"

#include <boost/lexical_cast.hpp>

using namespace org::esb::web;

class TestApp : public Wt::WApplication {
public:
  Wt::WBorderLayout * layout;
  Wt::Ext::Panel * cont;

  TestApp(const Wt::WEnvironment & env) : WApplication(env) {
    //    Wt::WContainerWidget*viewPort = new Wt::WContainerWidget(root());

    Wt::Ext::Container * c = new Wt::Ext::Container(root());
    Wt::WFitLayout * fitLayout = new Wt::WFitLayout();
    layout = new Wt::WBorderLayout();
    layout->setContentsMargins(0, 0, 0, 0);
    c->setLayout(layout);

    /*
    Wt::WContainerWidget*viewPort = new Wt::WContainerWidget();
    Wt::WBoxLayout *layout = new Wt::WBoxLayout(Wt::WBoxLayout::TopToBottom);
    viewPort->setLayout(layout);
    c->layout()->addWidget(viewPort);
     */
    //    c->setLayout(fitLayout);


    //    c->resize(Wt::WLength::Auto,500);

    Wt::WTimer *timer = new Wt::WTimer();
    timer->setInterval(5000);
    timer->timeout.connect(SLOT(this, TestApp::timeout));
    timer->start();


    SqlTable * t = new SqlTable("select * from files where parent =0");
    //    t->resize(Wt::WLength::Auto, Wt::WLength::Auto);

    cont = new Wt::Ext::Panel();
    Wt::WFitLayout * fit = new Wt::WFitLayout();
    cont->setLayout(fit);
    cont->layout()->addWidget(t);
    layout->addWidget(cont, Wt::WBorderLayout::Center);


    Wt::WText * south = new Wt::WText("South-side is best");
    south->resize(Wt::WLength::Auto, 30);

    Wt::WText * north = new Wt::WText("North-side is best");
    north->resize(Wt::WLength::Auto, 30);
    layout->addWidget(north, Wt::WBorderLayout::North);
    layout->addWidget(south, Wt::WBorderLayout::South);

    Wt::WText * east = new Wt::WText("East-side is best");
    east->resize(0, Wt::WLength::Auto);
    Wt::WText * west = new Wt::WText("East-side is best");
    west->resize(60, Wt::WLength::Auto);

    //    layout->addWidget(west, Wt::WBorderLayout::West);
    //    layout->addWidget(east, Wt::WBorderLayout::East);

  }

  void timeout() {
    Wt::WLayoutItem * item = NULL;

    if (cont->layout()->count() > 0) {
      Wt::WLayoutItem * item = cont->layout()->itemAt(0);
      cont->layout()->removeItem(item);
    }

    SqlTable * t = new SqlTable("select * from files");
    cont->layout()->addWidget(t);
    //    layout->update();
  }
};
