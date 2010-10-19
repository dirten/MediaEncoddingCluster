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
#include "Wt/Ext/Button"
#include "org/esb/lang/Ptr.h"
#include "../wtk/ComboBox.h"
#include "../wtk/ElementContainer.h"
#include "VideoAdvanceTableView.h"
#include <map>
namespace org {
  namespace esb {
    namespace web {

      class AudioPanel : public Wt::Ext::Panel {
      public:
        AudioPanel(std::map<std::string, std::string>&);
        virtual ~AudioPanel();
      private:
        std::map<std::string, std::string>&_parameter;
        void dataChanged();
        void setCodecGui(std::string codecid);
        Wt::Ext::Panel * main_panel;
        ComboBox * _codec;
        void codecSelected();
      };
    }
  }
}
#endif	/* AUDIOPANEL_H */

