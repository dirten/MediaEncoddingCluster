/* 
 * File:   VideoAdvanceTableView.h
 * Author: HoelscJ
 *
 * Created on 6. Oktober 2010, 13:02
 */

#ifndef VIDEOADVANCETABLEVIEW_H
#define	VIDEOADVANCETABLEVIEW_H
#include "Wt/Ext/TableView"
#include "Wt/Ext/Button"
#include "../wtk/ComboBox.h"
#include "Wt/Ext/LineEdit"
#include "org/esb/util/Log.h"

namespace org {
  namespace esb {
    namespace web {

      class VideoAdvanceTableView : public Wt::Ext::TableView {
        classlogger("org.esb.web.VideoAdvanceTableView")
      public:
        VideoAdvanceTableView(std::map<std::string, std::string>&, int flag);
        virtual ~VideoAdvanceTableView();
        void refresh();
        Wt::Signal<void> changed;
      private:
        std::map<std::string, std::string>&_dat;
        std::map<std::string, std::string> _options;
        void dataChanged(int data);
        void itemChanged(Wt::WStandardItem *data);
        void itemSelectionChangedMethod();
        void addOption();
        void removeOption();
        Wt::Ext::Button * removeOptionButton ;
        ComboBox * options;
        Wt::Ext::LineEdit * line;
        bool _dontChangeModel;
      };
    }
  }
}
#endif	/* VIDEOADVANCETABLEVIEW_H */

