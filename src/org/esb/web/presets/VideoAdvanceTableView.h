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
#include "Wt/Ext/ComboBox"
#include "Wt/Ext/LineEdit"

namespace org {
  namespace esb {
    namespace web {

      class VideoAdvanceTableView : public Wt::Ext::TableView {
      public:
        VideoAdvanceTableView(std::map<std::string, std::string>&);
        virtual ~VideoAdvanceTableView();
        void refresh();
        Wt::Signal<void> changed;
      private:
        std::map<std::string, std::string>&_dat;
        std::map<std::string, std::string> _options;
        void dataChanged(Wt::WModelIndex old, Wt::WModelIndex newdata);
        void itemChanged(Wt::WStandardItem *data);
        void itemSelectionChangedMethod();
        void addOption();
        void removeOption();
        Wt::Ext::Button * removeOptionButton ;
        Wt::Ext::ComboBox * options;
        Wt::Ext::LineEdit * line;
        bool _dontChangeModel;
      };
    }
  }
}
#endif	/* VIDEOADVANCETABLEVIEW_H */

