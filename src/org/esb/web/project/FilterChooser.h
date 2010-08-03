

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
        FilterChooser();
        ~FilterChooser();
        Wt::Signal<db::Filter> selected;
        Wt::Signal<void> canceled;
      private:
        void choose();
        void cancel();
        Ptr<FilterTable> _filter_table;

      };
    }
  }
}



#endif
