#ifndef ORG_ESB_WEB_BASEFILTERPANEL
#define ORG_ESB_WEB_BASEFILTERPANEL

#include "org/esb/lang/Ptr.h"
#include "Wt/Ext/Panel"
namespace org{
  namespace esb{
    namespace web{
      class BaseFilterPanel:public Wt::Ext::Panel{
      public:
        BaseFilterPanel():Wt::Ext::Panel(){}
        virtual ~BaseFilterPanel(){};
        virtual std::string getId()=0;
      };
    }
  }
}

#endif
