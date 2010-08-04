#include "FilterEditor.h"


#include "Wt/WBorderLayout"
#include "Wt/Ext/Button"

#include "FilterPanelFactory.h"

namespace org{
  namespace esb{
    namespace web{

      FilterEditor::FilterEditor(Ptr<db::Filter> filter):Wt::Ext::Dialog("Filter Editor"), _filter(filter){
        setLayout(new Wt::WBorderLayout());
        resize(600,400);
        _filterpanel=FilterPanelFactory::getFilterPanel(_filter->filterid);
        _filterpanel->setFilter(filter);
        _filterpanel->resize(300,Wt::WLength());
        ((Wt::WBorderLayout*)layout())->addWidget(_filterpanel,Wt::WBorderLayout::Center);
        _previewpanel=new PreviewPanel();
        _previewpanel->setProject(Ptr<db::Project>(new db::Project(filter->project().get().one())));
        _previewpanel->resize(300,Wt::WLength());
        ((Wt::WBorderLayout*)layout())->addWidget(_previewpanel,Wt::WBorderLayout::East);
        _previewpanel->preview();

        addButton(new Wt::Ext::Button("Save"));
        buttons().back()->clicked.connect(SLOT(this, FilterEditor::save));

        addButton(new Wt::Ext::Button("Cancel"));
        buttons().back()->clicked.connect(SLOT(this, Wt::Ext::Dialog::reject));

      }
      void FilterEditor::save(){
        _filterpanel->save();
        accept();
      }  
    }
  }
}
