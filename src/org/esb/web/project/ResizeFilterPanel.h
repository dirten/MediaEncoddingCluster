#ifndef ORG_ESB_WEB_RESIZEFILTERPANEL
#define ORG_ESB_WEB_RESIZEFILTERPANEL

#include "org/esb/lang/Ptr.h"
#include "BaseFilterPanel.h"
#include "wtk/ElementContainer.h"
namespace org{
  namespace esb{
    namespace web{
      class ResizeFilterPanel:public BaseFilterPanel{
      public:
        ResizeFilterPanel();
        ~ResizeFilterPanel();
        std::string getId();
        void setFilter(Ptr<db::Filter>);
        void save();
      private:
        Ptr<db::Filter> _filter;
        wtk::ElementContainer<Wt::Ext::LineEdit> _el;
      };
    }
  }
}

#endif
