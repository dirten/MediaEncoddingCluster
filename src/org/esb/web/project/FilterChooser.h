

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
        FilterChooser(std::map<std::string, std::string > available_filter);
        ~FilterChooser();
        Wt::Signal<void> selected;
        Wt::Signal<void> canceled;
        std::list<std::string> getSelectedFilter();
      private:
        void choose();
        void cancel();
        void enableButtons();
        FilterTable* _filter_table;
        std::map<std::string, std::string > _available_filters;
      };
    }
  }
}



#endif
