#include "FilterEditor.h"


#include "Wt/WFitLayout"
#include "Wt/Ext/Button"
#include "BaseFilterPanel.h"
#include "FilterPanelFactory.h"
namespace org{
  namespace esb{
    namespace web{

      FilterEditor::FilterEditor(Ptr<db::Filter> filter):Wt::Ext::Dialog("Filter Editor"), _filter(filter){
        setLayout(new Wt::WFitLayout());
        resize(600,400);
        BaseFilterPanel* panel=FilterPanelFactory::getFilterPanel(_filter->filterid);
        layout()->addWidget(panel);
        addButton(new Wt::Ext::Button("Save"));
        buttons().back()->clicked.connect(SLOT(this, Wt::Ext::Dialog::accept));

        addButton(new Wt::Ext::Button("Cancel"));
        buttons().back()->clicked.connect(SLOT(this, Wt::Ext::Dialog::reject));

      }
    }
  }
}
