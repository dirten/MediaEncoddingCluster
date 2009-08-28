#include "WebApp2.h"

#include <Wt/Ext/Container>
#include <Wt/WContainerWidget>
#include <Wt/WVBoxLayout>
#include <Wt/WFitLayout>
#include <Wt/WText>
#include <Wt/WLength>


namespace org {
  namespace esb {
    namespace web {

      WebApp2::WebApp2(const Wt::WEnvironment & env) : WApplication(env) {
        setTitle("Hive Webadmin");
        Wt::WContainerWidget *viewPort = new Wt::WContainerWidget(root());
//        viewPort->resize(Wt::WLength::Auto, Wt::WLength::Auto);

        Wt::WVBoxLayout *layout = new Wt::WVBoxLayout(viewPort);
        Wt::WText * t=new Wt::WText("One");
        layout->addWidget(t);
        layout->addWidget(new Wt::WText("Two"));
        layout->addWidget(new Wt::WText("Three"));
        layout->addWidget(new Wt::WText("Four"));



      }
    }
  }
}

