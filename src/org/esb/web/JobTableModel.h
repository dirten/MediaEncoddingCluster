/* 
 * File:   JobTableModel.h
 * Author: HoelscJ
 *
 * Created on 11. August 2010, 15:33
 */

#ifndef JOBTABLEMODEL_H
#define	JOBTABLEMODEL_H
#include "org/esb/db/hivedb.hpp"
#include <Wt/WStandardItemModel>

namespace org {
  namespace esb {
    namespace web {

      class JobTableModel :public Wt::WStandardItemModel{
      public:
        JobTableModel(std::vector<db::Job> jobs);
        JobTableModel(const JobTableModel& orig);
        virtual ~JobTableModel();
        void refresh(std::vector<db::Job> jobs);
      private:
        void setModelData(std::vector<db::Job> jobs);
      };

    }
  }
}

#endif	/* JOBTABLEMODEL_H */

