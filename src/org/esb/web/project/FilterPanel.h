#ifndef ORG_ESB_WEB_FILTERPANEL
#define ORG_ESB_WEB_FILTERPANEL

#include "org/esb/db/hivedb.hpp"
#include "FilterChooser.h"
#include "FilterEditor.h"

#include "Wt/Ext/Panel"
#include "Wt/Ext/Button"
#include "org/esb/lang/Ptr.h"

namespace org{
  namespace esb{
    namespace web{
      class FilterTable;
      class FilterPanel: public Wt::Ext::Panel{
      public:
        FilterPanel();
        ~FilterPanel();
        void setProject(Ptr<db::Project> p);
      private:
        void addFilter();
        void removeFilter();
        void editFilter();
        void enableButtons();
        void filterSelected(Ptr<db::Filter> filter);
        Ptr<FilterTable> _filter_table;
        Ptr<FilterChooser> _filter_chooser;
        Ptr<db::Project> _project;
        Ptr<FilterEditor> _filter_editor;
        std::vector<Ptr<db::Filter> > _available_filters;
        Wt::Ext::Button * removeFilterButton;
        Wt::Ext::Button * editFilterButton;
      };
    }
  }
}

#endif

