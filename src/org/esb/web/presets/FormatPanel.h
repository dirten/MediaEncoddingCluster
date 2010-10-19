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
#include "org/esb/web/wtk/ComboBox.h"
namespace org {
  namespace esb {
    namespace web {

      class FormatPanel : public Wt::Ext::Panel {
      public:
        FormatPanel(std::map<std::string, std::string>&);
        virtual ~FormatPanel();
        void save();
      private:
        void setFormatGui(std::string formatid);
        void setSelectedFormat();
        std::map<std::string, std::string> & _parameter;

        //Ptr<db::Profile>_profile;
        Wt::Ext::Panel * main_panel;
        ComboBox * _format;
      };
    }
  }
}

#endif	/* FORMATPANEL_H */

