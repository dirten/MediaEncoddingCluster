#include "ResizeFilterPanel.h"
#include "Wt/WContainerWidget"
#include "Wt/WGridLayout"
#include "Wt/WFitLayout"
#include "Wt/Ext/LineEdit"

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
        _el.addElement("width", "Output Width", "", l);
        _el.addElement("height", "Output Height", "", l);
        l->addWidget(new Wt::WText(), l->rowCount(), 0);
        l->setRowStretch(l->rowCount() - 1, -1);


        _el.getElement("height")->keyWentUp.connect(SLOT(this,ResizeFilterPanel::filterChanged));
        _el.getElement("width")->keyWentUp.connect(SLOT(this,ResizeFilterPanel::filterChanged));

      }

      ResizeFilterPanel::~ResizeFilterPanel(){

      }

      std::string ResizeFilterPanel::getId(){
        return "resize";
      }

      void ResizeFilterPanel::filterChanged(){
        save();
        changed.emit();
      }

      void ResizeFilterPanel::setFilter(Ptr<db::Filter> f){
        _filter=f;
          std::vector<db::FilterParameter> params=_filter->parameter().get().all();
          std::vector<db::FilterParameter>::iterator it=params.begin();
          for(;it!=params.end();it++){
            _el.getElement((*it).fkey)->setText((std::string)(*it).fval);
          }        
      }
      void ResizeFilterPanel::save(){
        if(_filter->parameter().get().count()==0){
          db::FilterParameter height(_filter->getDatabase());
          height.fkey="height";
          height.fval=_el.getElement("height")->text().narrow();
          height.update();
          _filter->parameter().link(height);
          db::FilterParameter width(_filter->getDatabase());
          width.fkey="width";
          width.fval=_el.getElement("width")->text().narrow();
          width.update();
          _filter->parameter().link(width);
        }else{
          std::vector<db::FilterParameter> params=_filter->parameter().get().all();
          std::vector<db::FilterParameter>::iterator it=params.begin();
          for(;it!=params.end();it++){
            (*it).fval=_el.getElement((*it).fkey)->text().narrow();
            (*it).update();
          }
        }
      }
    }
  }
}