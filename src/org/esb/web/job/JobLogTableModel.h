/* 
 * File:   JobLogTableModel.h
 * Author: HoelscJ
 *
 * Created on 17. August 2010, 16:17
 */

#ifndef JOBLOGTABLEMODEL_H
#define	JOBLOGTABLEMODEL_H
#include "Wt/WStandardItemModel"
#include "org/esb/db/hivedb.hpp"
namespace org {
  namespace esb {
    namespace web {

      class JobLogTableModel:public Wt::WStandardItemModel {
      public:
        JobLogTableModel();
        JobLogTableModel(const JobLogTableModel& orig);
        virtual ~JobLogTableModel();
        void setJob(db::Job);
      private:

      };
    }
  }
}
#endif	/* JOBLOGTABLEMODEL_H */

