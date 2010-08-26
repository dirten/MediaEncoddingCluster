#include "Wt/Ext/MessageBox"
#include "Projects.h"
#include "DbTable.h"
#include "Wt/WFitLayout"
#include "project/ProjectWizard.h"
#ifdef MessageBox
#undef MessageBox
#endif
namespace org {
  namespace esb {
    namespace web {

      Projects::Projects(Wt::WContainerWidget * parent ) : Wt::Ext::Container(parent){
        Wt::WFitLayout * l = new Wt::WFitLayout();
        setLayout(l);
        
        _table=new ProjectTable();
        layout()->addWidget(_table.get());

        _table->itemSelectionChanged.connect(SLOT(this, Projects::enableButtons));
        
        _table->setTopToolBar(new Wt::Ext::ToolBar());

        edit_button=_table->topToolBar()->addButton("Edit selected Project");
        edit_button->setIcon("icons/encoding-project-edit-icon.png");
        delete_button=_table->topToolBar()->addButton("Delete selected Project");
        delete_button->setIcon("icons/encoding-project-remove-icon.png");
        edit_button->setEnabled(false);
        delete_button->setEnabled(false);
        edit_button->clicked.connect(SLOT(this, Projects::editProject));
        delete_button->clicked.connect(SLOT(this, Projects::deleteProject));        
      }
      void Projects::editProject(){
        int c = atoi(boost::any_cast<string > (_table->model()->data(_table->selectedRows()[0], 0)).c_str());
        _wizard=Ptr<ProjectWizard>(new ProjectWizard());
        _wizard->saved.connect(SLOT(this,Projects::projectSaved));
        _wizard->open(c);
//        _wizard->show();

      }
      void Projects::createProject(){
        _wizard=Ptr<ProjectWizard>(new ProjectWizard());
        _wizard->saved.connect(SLOT(this,Projects::projectSaved));
        _wizard->open();

      }
      void Projects::deleteProject(){

        Wt::Ext::MessageBox *box=new Wt::Ext::MessageBox("really delete this Project?", "really",Wt::Warning,Wt::Ok|Wt::Cancel);
//        Wt::Ext::MessageBox *box=new Wt::Ext::MessageBox();
        box->show();
        if(box->exec()==Wt::Ext::Dialog::Accepted){
          int c = atoi(boost::any_cast<string > (_table->model()->data(_table->selectedRows()[0], 0)).c_str());
          db::HiveDb db("mysql", org::esb::config::Config::getProperty("db.url"));
          db::Project project = litesql::select<db::Project > (db, db::Project::Id == c).one();
          project.del();
          _table->refresh();
        }

      }
      void Projects::projectSaved(){
        _table->refresh();
      }
      void Projects::enableButtons(){
        if(_table->selectedRows().size()>0){
        edit_button->setEnabled(true);
        delete_button->setEnabled(true);
        }else{
        edit_button->setEnabled(false);
        delete_button->setEnabled(false);
        }
      }
    }
  }
}
