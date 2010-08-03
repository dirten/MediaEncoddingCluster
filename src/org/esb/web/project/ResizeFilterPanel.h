#ifndef ORG_ESB_WEB_RESIZEFILTERPANEL
#define ORG_ESB_WEB_RESIZEFILTERPANEL

#include "org/esb/lang/Ptr.h"
#include "BaseFilterPanel.h"
namespace org{
  namespace esb{
    namespace web{
      class ResizeFilterPanel:public BaseFilterPanel{
      public:
        ResizeFilterPanel();
        ~ResizeFilterPanel();
        std::string getId();
      };
    }
  }
}

#endif
