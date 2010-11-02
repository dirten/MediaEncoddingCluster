/* 
 * File:   ProjectTable.cpp
 * Author: HoelscJ
 * 
 * Created on 26. August 2010, 11:41
 */
#include "org/esb/db/hivedb.hpp"
#include "ProjectTable.h"
#include "Wt/WStandardItemModel"
#include "org/esb/util/StringUtil.h"
#include "org/esb/config/config.h"
#include "org/esb/hive/DatabaseService.h"
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
        db::HiveDb dbCon=org::esb::hive::DatabaseService::getDatabase();
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
      }

      void ProjectTable::refresh() {
        LOGDEBUG("ProjectTable::refresh()");

        db::HiveDb dbCon=org::esb::hive::DatabaseService::getDatabase();

        std::vector<db::Project> projects = litesql::select<db::Project > (dbCon).all();
        _model->refresh(projects);
      }

      ProjectTable::ProjectTable(const ProjectTable& orig) {
      }

      ProjectTable::~ProjectTable() {
      }
    }
  }
}
