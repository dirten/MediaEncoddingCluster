#ifndef ORG_ESB_WEB_RESIZEFILTERPANEL
#define ORG_ESB_WEB_RESIZEFILTERPANEL

#include "org/esb/lang/Ptr.h"
#include "BaseFilterPanel.h"
#include "wtk/ElementContainer.h"
#include "Wt/Ext/ComboBox"
#include <map>
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
        wtk::ElementContainer<Wt::Ext::ComboBox> _elcb;
        /*@TODO dat iss nicht schön, muss ausgetauscht werden*/
        std::map<std::string, int> _attribute_list;
        void filterChanged();
      };
    }
  }
}

#endif
