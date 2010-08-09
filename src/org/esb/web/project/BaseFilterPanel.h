#ifndef ORG_ESB_WEB_BASEFILTERPANEL
#define ORG_ESB_WEB_BASEFILTERPANEL
#include "org/esb/db/hivedb.hpp"
#include "org/esb/lang/Ptr.h"
#include "Wt/Ext/Panel"
#include "Wt/WSignal"
namespace org{
  namespace esb{
    namespace web{
      class BaseFilterPanel:public Wt::Ext::Panel{
      public:
        BaseFilterPanel():Wt::Ext::Panel(){}
        virtual ~BaseFilterPanel(){};
        virtual std::string getId()=0;
        virtual void setFilter(Ptr<db::Filter>)=0;
        virtual void save()=0;
        Wt::Signal<void> changed;
      };
    }
  }
}

#endif
