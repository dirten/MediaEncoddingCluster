/* 
 * File:   ProjectTable.cpp
 * Author: HoelscJ
 * 
 * Created on 26. August 2010, 11:41
 */
#include "org/esb/db/hivedb.hpp"
#include "ProjectTable.h"
#include "Wt/WStandardItemModel"
#include "Wt/Ext/ToolBar"
#include "org/esb/util/StringUtil.h"
#include "org/esb/config/config.h"
#include "org/esb/hive/DatabaseService.h"
#include "Wt/Ext/MessageBox"
/*this is a windows hack*/
#ifdef MessageBox
#undef MessageBox
#endif

namespace org {
  namespace esb {
    namespace web {

      class ProjectTableModel : public Wt::WStandardItemModel {
      public:

        ProjectTableModel(std::vector<db::Project> projects) : Wt::WStandardItemModel(0, 0, NULL) {
          insertColumns(0, 5);
          setHeaderData(0, std::string("Id"));
          setHeaderData(1, std::string("Name"));
          setHeaderData(2, std::string("#Files"));
          setHeaderData(3, std::string("#Profiles"));
          setHeaderData(4, std::string("Created"));
          setModelData(projects);
        }

        void refresh(std::vector<db::Project> projects) {
          setModelData(projects);
        }

        void setModelData(std::vector<db::Project> projects) {
          if (rowCount() < projects.size())
            insertRows(rowCount(), projects.size() - rowCount());
          if (rowCount() > projects.size()) {
            removeRows(0, rowCount() - projects.size());
          }
          for (int a = 0; a < projects.size(); a++) {
            db::Project pro = projects[a];
            setData(a, 0, org::esb::util::StringUtil::toString(pro.id.value()));
            setData(a, 1, pro.name.value());
            setData(a, 2, pro.mediafiles().get().count());
            setData(a, 3, pro.presets().get().count());
            setData(a, 4, pro.created.value().asString("%d-%m-%y %h:%M:%s"));
          }
        }
      };

      ProjectTable::ProjectTable() : Wt::Ext::TableView() {
        db::HiveDb dbCon = org::esb::hive::DatabaseService::getDatabase();
        std::vector<db::Project> projects = litesql::select<db::Project > (dbCon).all();
        _model = Ptr<ProjectTableModel > (new ProjectTableModel(projects));
        setModel(_model.get());
        setAlternatingRowColors(true);
        resizeColumnsToContents(true);
        setHighlightMouseOver(true);
        setSelectionBehavior(Wt::SelectRows);
        setSelectionMode(Wt::SingleSelection);
        setColumnWidth(0, 3);
        setAutoExpandColumn(1);
        setColumnWidth(2, 20);
        setColumnWidth(3, 20);
        setColumnWidth(4, 30);


        setTopToolBar(new Wt::Ext::ToolBar());
        create_button = topToolBar()->addButton("Create a new Project");
        create_button->setIcon("icons/encoding-project-add-icon.png");
        topToolBar()->addSeparator();
        edit_button = topToolBar()->addButton("Edit selected Project");
        edit_button->setIcon("icons/encoding-project-edit-icon.png");
        topToolBar()->addSeparator();
        delete_button = topToolBar()->addButton("Delete selected Project");
        delete_button->setIcon("icons/encoding-project-remove-icon.png");

        create_button->setEnabled(true);
        edit_button->setEnabled(false);
        delete_button->setEnabled(false);

        itemSelectionChanged().connect(SLOT(this, ProjectTable::enableButtons));
        create_button->clicked().connect(SLOT(this, ProjectTable::createProject));
        edit_button->clicked().connect(SLOT(this, ProjectTable::editProject));
        delete_button->clicked().connect(SLOT(this, ProjectTable::deleteProject));

      }

      void ProjectTable::refresh() {
        LOGDEBUG("ProjectTable::refresh()");
        db::HiveDb dbCon = org::esb::hive::DatabaseService::getDatabase();
        std::vector<db::Project> projects = litesql::select<db::Project > (dbCon).all();
        _model->refresh(projects);
      }

      void ProjectTable::editProject() {
        int c = atoi(boost::any_cast<string > (model()->data(selectedRows()[0], 0)).c_str());
        _wizard = Ptr<org::esb::web::v2::ProjectWizard > (new org::esb::web::v2::ProjectWizard());
        _wizard->saved.connect(SLOT(this, ProjectTable::projectSaved));
        _wizard->open(c);
        //_wizard->show();

      }

      void ProjectTable::createProject() {
        _wizard = Ptr<v2::ProjectWizard > (new v2::ProjectWizard());
        _wizard->saved.connect(SLOT(this, ProjectTable::projectSaved));
        _wizard->open();

      }

      void ProjectTable::deleteProject() {

        Wt::Ext::MessageBox *box = new Wt::Ext::MessageBox("really delete this Project?", "really", Wt::Warning, Wt::Ok | Wt::Cancel);
        //        Wt::Ext::MessageBox *box=new Wt::Ext::MessageBox();
        box->show();
        if (box->exec() == Wt::Ext::Dialog::Accepted) {
          int c = atoi(boost::any_cast<string > (model()->data(selectedRows()[0], 0)).c_str());
          db::HiveDb db = org::esb::hive::DatabaseService::getDatabase();
          db::Project project = litesql::select<db::Project > (db, db::Project::Id == c).one();
          project.del();
          refresh();
        }

      }
      void ProjectTable::projectSaved(){
        refresh();
      }

      void ProjectTable::enableButtons(){
        if(selectedRows().size()>0){
        edit_button->setEnabled(true);
        delete_button->setEnabled(true);
        }else{
        edit_button->setEnabled(false);
        delete_button->setEnabled(false);
        }
      }

      ProjectTable::~ProjectTable() {
      }
    }
  }
}
