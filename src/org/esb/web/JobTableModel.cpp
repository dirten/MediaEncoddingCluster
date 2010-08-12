/* 
 * File:   JobTableModel.cpp
 * Author: HoelscJ
 * 
 * Created on 11. August 2010, 15:33
 */

#include "JobTableModel.h"
namespace org {
  namespace esb {
    namespace web {

      JobTableModel::JobTableModel(std::vector<db::Job> jobs):Wt::WStandardItemModel(0, 0, NULL) {
        insertColumns(0, 7);
        setHeaderData(0, std::string("Input File"));
        setHeaderData(1, std::string("Output File"));
        setHeaderData(2, std::string("Enqueue Time"));
        setHeaderData(3, std::string("Start Time"));
        setHeaderData(4, std::string("End Time"));
        setHeaderData(5, std::string("Progress %"));
        setHeaderData(6, std::string("Status"));
        if (rowCount() < jobs.size())
          insertRows(rowCount(),jobs.size()-rowCount());
        for(int a=0;a<jobs.size();a++){
          db::Job job=jobs[a];
          setData(a,0,job.inputfile().get().one().filename.value());
          setData(a,1,job.outputfile().get().one().filename.value());
          setData(a,2,job.created.value().asString("%d-%m-%y %h:%M:%s"));
          setData(a,3,job.begintime.value().asString("%d-%m-%y %h:%M:%s"));
          setData(a,4,job.endtime.value().asString("%d-%m-%y %h:%M:%s"));
          setData(a,5,job.progress.value());
          setData(a,6,job.status.value());
        }
      }

      JobTableModel::JobTableModel(const JobTableModel& orig) {
      }

      JobTableModel::~JobTableModel() {
      }
    }
  }
}

