/* 
 * File:   JobTable.h
 * Author: HoelscJ
 *
 * Created on 13. August 2010, 17:32
 */

#ifndef JOBTABLE_H
#define	JOBTABLE_H

#include <Wt/Ext/TableView>
#include "org/esb/lang/Ptr.h"

namespace org {
  namespace esb {
    namespace web {
      class JobTableModel;
      class JobTable:public Wt::Ext::TableView {
      public:
        JobTable();
        JobTable(const JobTable& orig);
        virtual ~JobTable();

      private:
        Ptr<JobTableModel> _model;
        void refresh();
      };

    }
  }
}

#endif	/* JOBTABLE_H */

