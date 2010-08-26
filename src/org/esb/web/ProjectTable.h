/* 
 * File:   ProjectTable.h
 * Author: HoelscJ
 *
 * Created on 26. August 2010, 11:41
 */

#ifndef PROJECTTABLE_H
#define	PROJECTTABLE_H

#include "Wt/Ext/TableView"
#include "org/esb/lang/Ptr.h"
namespace org {
  namespace esb {
    namespace web {
      class ProjectTableModel;
      class ProjectTable:public Wt::Ext::TableView {
      public:
        ProjectTable();
        ProjectTable(const ProjectTable& orig);
        virtual ~ProjectTable();
        void refresh();
      private:
        Ptr<ProjectTableModel> _model;
      };
    }
  }
}

#endif	/* PROJECTTABLE_H */

