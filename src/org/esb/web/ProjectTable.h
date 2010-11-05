/* 
 * File:   ProjectTable.h
 * Author: HoelscJ
 *
 * Created on 26. August 2010, 11:41
 */

#ifndef PROJECTTABLE_H
#define	PROJECTTABLE_H

#include "Wt/Ext/TableView"
#include "Wt/Ext/Button"
#include "org/esb/lang/Ptr.h"
#include "org/esb/web/project_v2/ProjectWizard.h"
namespace org {
  namespace esb {
    namespace web {
      class ProjectTableModel;
      class ProjectTable:public Wt::Ext::TableView {
      public:
        ProjectTable();
        virtual ~ProjectTable();
        void refresh();
        void createProject();
      private:
        void editProject();
        void deleteProject();
        void enableButtons();
        void projectSaved();

        Ptr<ProjectTableModel> _model;
        Wt::Ext::Button * create_button;
        Wt::Ext::Button * edit_button;
        Wt::Ext::Button * delete_button;
        Ptr<v2::ProjectWizard> _wizard;

      };
    }
  }
}

#endif	/* PROJECTTABLE_H */

