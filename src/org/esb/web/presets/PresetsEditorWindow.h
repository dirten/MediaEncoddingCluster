/* 
 * File:   PresetsEditorWindow.h
 * Author: HoelscJ
 *
 * Created on 4. Oktober 2010, 11:56
 */

#ifndef PRESETSEDITORWINDOW_H
#define	PRESETSEDITORWINDOW_H
#include "org/esb/db/hivedb.hpp"
#include "org/esb/lang/Ptr.h"

#include "Wt/Ext/Dialog"
#include "Wt/WWidget"
namespace org {
  namespace esb {
    namespace web {

      class PresetsEditorWindow: public Wt::Ext::Dialog {
      public:
        PresetsEditorWindow(Ptr<db::Profile>);
        virtual ~PresetsEditorWindow();
      private:
        void cancel();
        void save();

        //db::HiveDb _db;
        Ptr<db::Profile>_profile;

      };
    }
  }
}

#endif	/* PRESETSEDITORWINDOW_H */

