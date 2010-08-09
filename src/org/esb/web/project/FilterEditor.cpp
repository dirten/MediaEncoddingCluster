#include "FilterEditor.h"


#include "Wt/WBorderLayout"
#include "Wt/Ext/Button"

#include "FilterPanelFactory.h"
#include "org/esb/util/Log.h"

namespace org{
  namespace esb{
    namespace web{

      FilterEditor::FilterEditor(Ptr<db::Filter> filter):Wt::Ext::Dialog("Filter Editor"), _filter(filter){
        LOGDEBUG("FilterEditor::FilterEditor(Ptr<db::Filter> filter)");
        setLayout(new Wt::WBorderLayout());
        resize(900,560);
        _filterpanel=FilterPanelFactory::getFilterPanel(_filter->filterid);
        _filterpanel->setFilter(filter);
        _filterpanel->changed.connect(SLOT(this,FilterEditor::filterChanged));
//        _filterpanel->resize(300,Wt::WLength());
        ((Wt::WBorderLayout*)layout())->addWidget(_filterpanel,Wt::WBorderLayout::Center);
        
        _previewpanel=new PreviewPanel();
        ((Wt::WBorderLayout*)layout())->addWidget(_previewpanel,Wt::WBorderLayout::East);
        _previewpanel->setProject(Ptr<db::Project>(new db::Project(filter->project().get().one())));
        _previewpanel->resize(540,Wt::WLength());
        _previewpanel->setResizable(true);

        _previewpanel->preview();

        addButton(new Wt::Ext::Button("Save"));
        buttons().back()->clicked.connect(SLOT(this, FilterEditor::save));

        addButton(new Wt::Ext::Button("Cancel"));
        buttons().back()->clicked.connect(SLOT(this, Wt::Ext::Dialog::reject));

      }
      FilterEditor::~FilterEditor(){
        LOGDEBUG("FilterEditor::~FilterEditor()");
        delete _previewpanel;
      }

      void FilterEditor::filterChanged(){
        _previewpanel->filterChanged();
      }

      void FilterEditor::save(){
        _filterpanel->save();
        accept();
      }  
    }
  }
}
