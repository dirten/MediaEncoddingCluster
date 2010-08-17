/* 
 * File:   JobLogTableModel.cpp
 * Author: HoelscJ
 * 
 * Created on 17. August 2010, 16:17
 */

#include "JobLogTableModel.h"
namespace org {
  namespace esb {
    namespace web {

      JobLogTableModel::JobLogTableModel():Wt::WStandardItemModel() {
        insertColumns(0,2);
        setHeaderData(0, std::string("Created"));
        setHeaderData(1, std::string("Message"));
      }

      JobLogTableModel::JobLogTableModel(const JobLogTableModel& orig) {
      }

      JobLogTableModel::~JobLogTableModel() {
      }

      void JobLogTableModel::setJob(db::Job job) {
        std::vector<db::JobLog> logs=job.joblog().get().all();

        if (rowCount() < logs.size())
          insertRows(rowCount(),logs.size()-rowCount());
        for(int a=0;a<logs.size();a++){
          db::JobLog log=logs[a];
          setData(a,0,log.created.value().asString("%d-%m-%y %h:%M:%s"));
          setData(a,1,log.message.value());
        }
      }
    }
  }
}
