#ifndef ORG_ESB_WEB_FILTERPANELFACTORY
#define ORG_ESB_WEB_FILTERPANELFACTORY

#include "org/esb/lang/Ptr.h"
#include "BaseFilterPanel.h"
#include <set>
namespace org{
  namespace esb{
    namespace web{
      class FilterPanelFactory{
      private:
        FilterPanelFactory();
        ~FilterPanelFactory();
        static std::set<BaseFilterPanel *> _filter_map;
      public:
        static BaseFilterPanel *getFilterPanel(std::string id);
      };
    }
  }
}

#endif
