#include "FilterPanel.h"

#include "Wt/WFitLayout"
#include "Wt/Ext/Toolbar"
namespace org{
  namespace esb{
    namespace web{

      FilterPanel::FilterPanel():Wt::Ext::Panel(){
        setTitle("Filter");
        setLayout(new Wt::WFitLayout());
        layout()->setContentsMargins(0, 0, 0, 0);


        setTopToolBar(new Wt::Ext::ToolBar());

        Wt::Ext::Button * addFilterButton = topToolBar()->addButton("Add Filter");
        Wt::Ext::Button * removeFilterButton = topToolBar()->addButton("Remove Filter");

        addFilterButton->clicked.connect(SLOT(this, FilterPanel::addFilter));
//        removeVideoButton->clicked.connect(SLOT(this, InputFilePanel::removeVideo));
      }

      FilterPanel::~FilterPanel(){
        
      }

      void FilterPanel::addFilter(){
        _filter_chooser=Ptr<FilterChooser>(new FilterChooser());
        _filter_chooser->show();
      }
    }
  }
}

