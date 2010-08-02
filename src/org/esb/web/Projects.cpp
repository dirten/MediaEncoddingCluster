#include "Projects.h"
#include "DbTable.h"
#include "Wt/WFitLayout"
#include "project/ProjectWizard.h"

namespace org {
  namespace esb {
    namespace web {

      Projects::Projects(Wt::WContainerWidget * parent ) : Wt::Ext::Container(parent){
        Wt::WFitLayout * l = new Wt::WFitLayout();
        setLayout(l);
        
        list<ColumnConfig> columnConfigs;
        columnConfigs.push_back(ColumnConfig(db::Project::Id,"Id" ,20));
        columnConfigs.push_back(ColumnConfig(db::Project::Name,"Name" ,200));
        columnConfigs.push_back(ColumnConfig(db::Project::Created,"Created" ,300));
        _table=Ptr<DbTable>(new DbTable(columnConfigs,litesql::Expr()));
        layout()->addWidget(_table.get());

        _table->itemSelectionChanged.connect(SLOT(this, Projects::enableButtons));
        
        _table->setTopToolBar(new Wt::Ext::ToolBar());

        edit_button=_table->topToolBar()->addButton("Edit selected Project");
        delete_button=_table->topToolBar()->addButton("Delete selected Project");
        edit_button->setEnabled(false);
        delete_button->setEnabled(false);
        edit_button->clicked.connect(SLOT(this, Projects::editProject));
        delete_button->clicked.connect(SLOT(this, Projects::deleteProject));        
      }
      void Projects::editProject(){
        int c = atoi(boost::any_cast<string > (_table->getModel()->data(_table->selectedRows()[0], 0)).c_str());
        _wizard=Ptr<ProjectWizard>(new ProjectWizard());
        _wizard->open(c);
        _wizard->show();

      }
      void Projects::deleteProject(){
      }
      void Projects::enableButtons(){
        edit_button->setEnabled(true);
        delete_button->setEnabled(true);
      }
    }
  }
}
