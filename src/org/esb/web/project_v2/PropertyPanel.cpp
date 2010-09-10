/* 
 * File:   PropertyPanel.cpp
 * Author: HoelscJ
 * 
 * Created on 10. September 2010, 16:41
 */



#include "PropertyPanel.h"
#include "Wt/WAccordionLayout"
#include "org/esb/web/project/ProfilePanel.h"
#include "org/esb/web/project/FilterPanel.h"
#include "org/esb/web/project/ProjectPropertyPanel.h"
namespace org {
  namespace esb {
    namespace web {

      PropertyPanel::PropertyPanel() : Wt::Ext::Panel() {
        setLayout(new Wt::WAccordionLayout());

        this->layout()->addWidget(new org::esb::web::ProfilePanel());

        this->layout()->addWidget(new org::esb::web::FilterPanel());

        this->layout()->addWidget(new org::esb::web::ProjectPropertyPanel());
      }

      PropertyPanel::PropertyPanel(const PropertyPanel& orig) {
      }

      PropertyPanel::~PropertyPanel() {
      }

      void PropertyPanel::save() {
        ((org::esb::web::ProjectPropertyPanel*)this->layout()->itemAt(2)->widget())->save();
      }

      void PropertyPanel::setProject(Ptr<db::Project> p) {
        ((org::esb::web::ProfilePanel*)this->layout()->itemAt(0)->widget())->setProject(p);
        ((org::esb::web::FilterPanel*)this->layout()->itemAt(1)->widget())->setProject(p);
        ((org::esb::web::ProjectPropertyPanel*)this->layout()->itemAt(2)->widget())->setProject(p);
      }
    }
  }
}
