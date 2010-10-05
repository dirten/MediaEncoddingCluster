/* 
 * File:   FormatPanel.h
 * Author: HoelscJ
 *
 * Created on 4. Oktober 2010, 17:10
 */

#ifndef FORMATPANEL_H
#define	FORMATPANEL_H
#include "org/esb/db/hivedb.hpp"
#include "org/esb/lang/Ptr.h"
#include "Wt/Ext/Panel"
#include "../wtk/ElementContainer.h"
#include "Wt/Ext/LineEdit"
#include "Wt/Ext/ComboBox"
namespace org {
  namespace esb {
    namespace web {

      class FormatPanel : public Wt::Ext::Panel {
      public:
        FormatPanel(Ptr<db::Profile>);
        virtual ~FormatPanel();
      private:
        Ptr<db::Profile>_profile;
        wtk::ElementContainer<Wt::Ext::LineEdit> _el;
        wtk::ElementContainer<Wt::Ext::ComboBox> _elcb;
        void setAvailableExtensions();

      };
    }
  }
}

#endif	/* FORMATPANEL_H */

