#include "FilterEditor.h"


#include "Wt/WBorderLayout"
#include "Wt/Ext/Button"
#include "BaseFilterPanel.h"
#include "FilterPanelFactory.h"
#include "PreviewPanel.h"
namespace org{
  namespace esb{
    namespace web{

      FilterEditor::FilterEditor(Ptr<db::Filter> filter):Wt::Ext::Dialog("Filter Editor"), _filter(filter){
        setLayout(new Wt::WBorderLayout());
        resize(600,400);
        BaseFilterPanel* panel=FilterPanelFactory::getFilterPanel(_filter->filterid);
        panel->setFilter(filter);
        panel->resize(300,Wt::WLength());
        ((Wt::WBorderLayout*)layout())->addWidget(panel,Wt::WBorderLayout::Center);
        PreviewPanel * preview=new PreviewPanel();
        preview->resize(300,Wt::WLength());
        ((Wt::WBorderLayout*)layout())->addWidget(preview,Wt::WBorderLayout::East);
        

        addButton(new Wt::Ext::Button("Save"));
        buttons().back()->clicked.connect(SLOT(this, Wt::Ext::Dialog::accept));

        addButton(new Wt::Ext::Button("Cancel"));
        buttons().back()->clicked.connect(SLOT(this, Wt::Ext::Dialog::reject));

      }
    }
  }
}
