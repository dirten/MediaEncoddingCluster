/* 
 * File:   PropertyPanel.h
 * Author: HoelscJ
 *
 * Created on 10. September 2010, 16:41
 */

#ifndef PROPERTYPANEL_H
#define	PROPERTYPANEL_H
#include "org/esb/db/hivedb.hpp"

#include "Wt/Ext/Panel"
#include "org/esb/lang/Ptr.h"
namespace org {
  namespace esb {
    namespace web {
      class PropertyPanel : public Wt::Ext::Panel {
      public:
        PropertyPanel();
        PropertyPanel(const PropertyPanel& orig);
        virtual ~PropertyPanel();
        void setProject(Ptr<db::Project>);
        void save();
      private:
      };
    }
  }
}

#endif	/* PROPERTYPANEL_H */

