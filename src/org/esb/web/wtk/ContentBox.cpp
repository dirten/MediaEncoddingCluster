#include "ContentBox.h"
namespace org {
  namespace esb {
    namespace web {
      namespace wtk {

        ContentBox::ContentBox(const char * name,WContainerWidget *parent): Div(name){
//          setId(name);
          Div * t1=new Div("t");
          Div * t2=new Div("t");
          Div * t3=new Div("t");

          Div * b1=new Div("b");
          Div * b2=new Div("b");
          Div * b3=new Div("b");

          t1->addWidget(t2);
          t2->addWidget(t3);

          b1->addWidget(b2);
          b2->addWidget(b3);

          addWidget(t1);
          
          m=new Div("m");
          
          addWidget(m);
          
          addWidget(b1);
        }
        
        void ContentBox::setHeaderText(std::string  text){
        }
        void ContentBox::setContent(Wt::WWidget * widget){
          m->addWidget(widget);
        }
        
        void ContentBox::resize(const Wt::WLength &width, const Wt::WLength &height){
          Div::resize(width,height);
        }        
      }
    }
  }
}
