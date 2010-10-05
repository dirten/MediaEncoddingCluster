/* 
 * File:   AudioPanel.h
 * Author: HoelscJ
 *
 * Created on 4. Oktober 2010, 17:13
 */

#ifndef AUDIOPANEL_H
#define	AUDIOPANEL_H
#include "org/esb/db/hivedb.hpp"
#include "Wt/Ext/Panel"
namespace org {
  namespace esb {
    namespace web {

      class AudioPanel:public Wt::Ext::Panel {
      public:
        AudioPanel(db::Profile&);
        virtual ~AudioPanel();
      private:
        db::Profile & _profile;
      };
    }
  }
}
#endif	/* AUDIOPANEL_H */

