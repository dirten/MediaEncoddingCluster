/* 
 * File:   PropertyPanel.cpp
 * Author: HoelscJ
 * 
 * Created on 10. September 2010, 16:41
 */



#include "PropertyPanel.h"
#include "Wt/WAccordionLayout"
#include "Wt/WFitLayout"
#include "Wt/Ext/TabWidget"
namespace org {
  namespace esb {
    namespace web {

      PropertyPanel::PropertyPanel() : Wt::Ext::Panel() {
        setLayout(new Wt::WFitLayout());
        //setLayout(new Wt::WAccordionLayout());
        tab=new Wt::Ext::TabWidget();

        this->layout()->addWidget(tab);
        tab->addTab(_file_panel=new org::esb::web::InputFilePanel(),"Input Files");
        tab->addTab(_profile_panel=new org::esb::web::ProfilePanel(),"Output Profiles");
        tab->addTab(_prop_panel=new org::esb::web::ProjectPropertyPanel(),"Project Properties");
//        this->layout()->addWidget(new org::esb::web::ProfilePanel());

        //this->layout()->addWidget(new org::esb::web::FilterPanel());

//        this->layout()->addWidget(new org::esb::web::ProjectPropertyPanel());
      }

      PropertyPanel::PropertyPanel(const PropertyPanel& orig) {
      }

      PropertyPanel::~PropertyPanel() {
      }

      void PropertyPanel::save() {
        _prop_panel->save();
      }

      void PropertyPanel::setProject(Ptr<db::Project> p) {
        _file_panel->setProject(p);
        _profile_panel->setProject(p);
        _prop_panel->setProject(p);
      }
    }
  }
}
