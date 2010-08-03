#ifndef ORG_ESB_WEB_DEINTERLACEFILTERPANEL
#define ORG_ESB_WEB_DEINTERLACEFILTERPANEL

#include "org/esb/lang/Ptr.h"
#include "BaseFilterPanel.h"
namespace org{
  namespace esb{
    namespace web{
      class DeinterlaceFilterPanel:public BaseFilterPanel{
      public:
        DeinterlaceFilterPanel();
        ~DeinterlaceFilterPanel();
        std::string getId();
      };
    }
  }
}

#endif
