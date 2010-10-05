/* 
 * File:   VideoPanel.h
 * Author: HoelscJ
 *
 * Created on 4. Oktober 2010, 17:12
 */

#ifndef VIDEOPANEL_H
#define	VIDEOPANEL_H
#include "org/esb/db/hivedb.hpp"
#include "org/esb/lang/Ptr.h"
#include "Wt/Ext/Panel"
#include "Wt/Ext/Button"
#include "Wt/Ext/LineEdit"
#include "Wt/Ext/ComboBox"
#include "../wtk/ElementContainer.h"
#include <map>
namespace org {
  namespace esb {
    namespace web {

      class VideoPanel : public Wt::Ext::Panel {
      public:
        VideoPanel(Ptr<db::Profile>);
        virtual ~VideoPanel();
      private:
        Ptr<db::Profile>_profile;
        Wt::Ext::Button *_advanced;
        std::list<int> _bitrates;
        std::list<int> _keyframes;
        std::list<std::string> _methods;
        std::map<std::string, std::string> _parameter;
        wtk::ElementContainer<Wt::Ext::LineEdit> _el;
        wtk::ElementContainer<Wt::Ext::ComboBox> _elcb;
        void setPredefinedCodecFlags();
        void setSelectedPredifinedCodecFlags();
        void switchAdvanced();
      };
    }
  }
}
#endif	/* VIDEOPANEL_H */

