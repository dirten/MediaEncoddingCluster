#include "ResizeFilterPanel.h"
#include "Wt/WContainerWidget"
#include "Wt/WGridLayout"
#include "Wt/WFitLayout"
#include "Wt/Ext/LineEdit"
#include "org/esb/util/StringUtil.h"
#include "org/esb/util/Log.h"
namespace org{
  namespace esb{
    namespace web{

      ResizeFilterPanel::ResizeFilterPanel():BaseFilterPanel(){
        setTitle("Resize Filter");
        setLayout(new Wt::WFitLayout());
        _attribute_list["aspect_ratio"]=0;
        _attribute_list["width"]=0;
        _attribute_list["height"]=0;
        Wt::WContainerWidget * cont = new Wt::WContainerWidget();
        Wt::WGridLayout * l = new Wt::WGridLayout();
        cont->setLayout(l);
        layout()->addWidget(cont);
        
        ComboBox * v_aspect = _elcb.addElement("aspect_ratio", "Aspect Ratio", "", l);
        v_aspect->setTextSize(50);
        v_aspect->addItem("Rescale to width and height defined below");
        v_aspect->addItem("Keep Aspect Ratio from Source");
        
        _el.addElement("width", "Output Width", "", l);
        _el.addElement("height", "Output Height", "", l);
        l->addWidget(new Wt::WText(), l->rowCount(), 0);
        l->setRowStretch(l->rowCount() - 1, -1);


        _el.getElement("height")->keyWentUp().connect(SLOT(this,ResizeFilterPanel::filterChanged));
        _el.getElement("width")->keyWentUp().connect(SLOT(this,ResizeFilterPanel::filterChanged));
        _elcb.getElement("aspect_ratio")->activated().connect(SLOT(this,ResizeFilterPanel::filterChanged));

      }

      ResizeFilterPanel::~ResizeFilterPanel(){

      }

      std::string ResizeFilterPanel::getId(){
        return "resize";
      }

      void ResizeFilterPanel::filterChanged(){
        LOGDEBUG("void ResizeFilterPanel::filterChanged()");
        save();
        changed.emit();
      }

      void ResizeFilterPanel::setFilter(Ptr<db::Filter> f){

        _filter=f;
          std::vector<db::FilterParameter> params=_filter->parameter().get().all();
          std::vector<db::FilterParameter>::iterator it=params.begin();
          for(;it!=params.end();it++){
            _attribute_list[(*it).fkey]=1;
            if(_el.contains((*it).fkey)){
            _el.getElement((*it).fkey)->setText((std::string)(*it).fval);
            }else if(_elcb.contains((*it).fkey)){
              _elcb.getElement((*it).fkey)->setCurrentIndex(atoi((*it).fval.value().c_str()));
            }
          }
          std::map<std::string, int>::iterator it_att=_attribute_list.begin();
          for(;it_att!=_attribute_list.end();it_att++){
            if((*it_att).second==0){
              db::FilterParameter param(_filter->getDatabase());
              param.fkey=(*it_att).first;
              param.update();
              _filter->parameter().link(param);
            }
          }
      }
      void ResizeFilterPanel::save(){
        if(atoi(_el.getElement("height")->text().narrow().c_str())==0||atoi(_el.getElement("width")->text().narrow().c_str())==0)
          return;
        if(_filter->parameter().get().count()==0){
          /*
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
          db::FilterParameter aspect_ratio(_filter->getDatabase());
          aspect_ratio.fkey="aspect_ratio";
          aspect_ratio.fval=_elcb.getElement("aspect_ratio")->currentIndex();
          aspect_ratio.update();
          _filter->parameter().link(aspect_ratio);
           */
        }else{
          std::vector<db::FilterParameter> params=_filter->parameter().get().all();
          std::vector<db::FilterParameter>::iterator it=params.begin();
          for(;it!=params.end();it++){
            if(_el.contains((*it).fkey)){
              (*it).fval=_el.getElement((*it).fkey)->text().narrow();
            }else if(_elcb.contains((*it).fkey.value())){
              (*it).fval=org::esb::util::StringUtil::toString(_elcb.getElement((*it).fkey)->currentIndex());
            }
            (*it).update();
          }
        }
      }
    }
  }
}