/* 
 * File:   FilterPanel.h
 * Author: HoelscJ
 *
 * Created on 4. Oktober 2010, 17:13
 */

#ifndef FILTERPANEL_H
#define	FILTERPANEL_H
#include "org/esb/db/hivedb.hpp"
#include "Wt/Ext/Panel"
namespace org {
  namespace esb {
    namespace web {

      class FilterPanel : public Wt::Ext::Panel {
      public:
        FilterPanel(db::Profile&);
        virtual ~FilterPanel();
      private:
        db::Profile & _profile;
      };
    }
  }
}
#endif	/* FILTERPANEL_H */

