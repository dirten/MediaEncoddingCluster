/* 
 * File:   PresetsEditor.h
 * Author: HoelscJ
 *
 * Created on 4. Oktober 2010, 11:52
 */

#ifndef PRESETSEDITOR_H
#define	PRESETSEDITOR_H
#include "org/esb/db/hivedb.hpp"
#include "org/esb/lang/Ptr.h"
#include "Wt/Ext/Panel"
namespace org {
  namespace esb {
    namespace web {

      class PresetsEditor:public Wt::Ext::Panel {
      public:
        PresetsEditor(Ptr<db::Profile>);
        virtual ~PresetsEditor();
      private:
        Wt::WWidget * createTop();
        Wt::WWidget * createContent();
        Ptr<db::Profile>_profile;
      };
    }
  }
}

#endif	/* PRESETSEDITOR_H */

