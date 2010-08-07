

#ifndef ORG_ESB_WEB_PROFILECHOOSER
#define ORG_ESB_WEB_PROFILECHOOSER
#include "org/esb/db/hivedb.hpp"
#include "Wt/Ext/Dialog"
#include "Wt/WSignal"
#include "org/esb/lang/Ptr.h"

namespace org{
  namespace esb{
    namespace web{
      class FilterTable;
      class FilterChooser: public Wt::Ext::Dialog{
      public:
        FilterChooser(std::vector<Ptr<db::Filter> > available_filter);
        ~FilterChooser();
        Wt::Signal<Ptr<db::Filter> > selected;
        Wt::Signal<void> canceled;
      private:
        void choose();
        void cancel();
        void enableButtons();
        Ptr<FilterTable> _filter_table;
        std::vector<Ptr<db::Filter> > _available_filters;
      };
    }
  }
}



#endif