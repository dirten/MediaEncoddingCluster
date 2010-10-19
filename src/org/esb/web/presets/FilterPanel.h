/* 
 * File:   FilterPanel.h
 * Author: HoelscJ
 *
 * Created on 11. Oktober 2010, 14:29
 */

#ifndef FILTERPANEL_H
#define	FILTERPANEL_H
#include "org/esb/db/hivedb.hpp"
#include "org/esb/lang/Ptr.h"

#include "Wt/Ext/Panel"
#include "org/esb/web/presets/FilterTable.h"

namespace org {
  namespace esb {
    namespace web {

      class PresetFilterPanel : public Wt::Ext::Panel {
      public:
        PresetFilterPanel(Ptr<db::Profile>);
        virtual ~PresetFilterPanel();
        void save();
      private:
        Ptr<db::Profile>_profile;
        Ptr<org::esb::web::PresetFilterTable >filter_table;
        Wt::Ext::Panel* _props;
        Wt::WContainerWidget* _cont;
        void filterSelected();
        Wt::WGridLayout * grid;
        std::map<std::string, std::string> _parameter;
      };
    }
  }
}

#endif	/* FILTERPANEL_H */

