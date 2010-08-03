#ifndef ORG_ESB_WEB_FILTEREDITOR
#define ORG_ESB_WEB_FILTEREDITOR

#include "org/esb/db/hivedb.hpp"
#include "Wt/Ext/Dialog"
#include "org/esb/lang/Ptr.h"

namespace org{
  namespace esb{
    namespace web{

      class FilterEditor:public Wt::Ext::Dialog{
      public:
        FilterEditor(Ptr<db::Filter> filter);
      private:
        Ptr<db::Filter> _filter;
      };
    }
  }
}

#endif
