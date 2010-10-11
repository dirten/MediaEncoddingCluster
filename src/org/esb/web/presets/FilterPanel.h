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

namespace org {
  namespace esb {
    namespace web {

      class PresetFilterPanel : public Wt::Ext::Panel {
      public:
        PresetFilterPanel(Ptr<db::Profile>);
        virtual ~PresetFilterPanel();
      private:
          std::vector<Ptr<db::Filter> > _available_filter;
      };
    }
  }
}

#endif	/* FILTERPANEL_H */

