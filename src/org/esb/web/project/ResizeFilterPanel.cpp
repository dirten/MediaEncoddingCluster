#include "ResizeFilterPanel.h"
#include "Wt/WContainerWidget"
#include "Wt/WGridLayout"
#include "Wt/WFitLayout"

namespace org{
  namespace esb{
    namespace web{

      ResizeFilterPanel::ResizeFilterPanel():BaseFilterPanel(){
        setTitle("Resize Filter");
        setLayout(new Wt::WFitLayout());

        Wt::WContainerWidget * cont = new Wt::WContainerWidget();
        Wt::WGridLayout * l = new Wt::WGridLayout();
        cont->setLayout(l);
        layout()->addWidget(cont);
        _el.addElement("height", "Output Height", "", l);
        _el.addElement("width", "Output Width", "", l);
        l->addWidget(new Wt::WText(), l->rowCount(), 0);
        l->setRowStretch(l->rowCount() - 1, -1);
      }
      ResizeFilterPanel::~ResizeFilterPanel(){

      }
      std::string ResizeFilterPanel::getId(){
        return "resize";
      }
      void ResizeFilterPanel::setFilter(Ptr<db::Filter> f){
        _filter=f;
      }

    }
  }
}