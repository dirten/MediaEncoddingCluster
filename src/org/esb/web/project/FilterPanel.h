#ifndef ORG_ESB_WEB_FILTERPANEL
#define ORG_ESB_WEB_FILTERPANEL

#include "FilterChooser.h"

#include "Wt/Ext/Panel"
#include "org/esb/lang/Ptr.h"

namespace org{
  namespace esb{
    namespace web{

      class FilterPanel: public Wt::Ext::Panel{
      public:
        FilterPanel();
        ~FilterPanel();
      private:
        void addFilter();
        Ptr<FilterChooser> _filter_chooser;
      };
    }
  }
}

#endif

