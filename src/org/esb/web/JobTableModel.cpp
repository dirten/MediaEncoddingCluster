/* 
 * File:   JobTableModel.cpp
 * Author: HoelscJ
 * 
 * Created on 11. August 2010, 15:33
 */

#include "JobTableModel.h"
#include "org/esb/util/StringUtil.h"
namespace org {
  namespace esb {
    namespace web {

      JobTableModel::JobTableModel(std::vector<db::Job> jobs):Wt::WStandardItemModel(0, 0, NULL) {
        insertColumns(0, 8);
        setHeaderData(0, std::string("Id"));
        setHeaderData(1, std::string("Input File"));
        setHeaderData(2, std::string("Output File"));
        setHeaderData(3, std::string("Enqueue Time"));
        setHeaderData(4, std::string("Start Time"));
        setHeaderData(5, std::string("End Time"));
        setHeaderData(6, std::string("Progress %"));
        setHeaderData(7, std::string("Status"));
        setModelData(jobs);
      }

      void  JobTableModel::refresh(std::vector<db::Job> jobs){
        setModelData(jobs);
      }
      
      void  JobTableModel::setModelData(std::vector<db::Job> jobs){
        if (rowCount() < jobs.size())
          insertRows(rowCount(),jobs.size()-rowCount());
        for(int a=0;a<jobs.size();a++){
          db::Job job=jobs[a];
          setData(a,0,org::esb::util::StringUtil::toString(job.id.value()));
          setData(a,1,"         &nbsp;"+job.infile.value()+"");
          setData(a,2,job.outfile.value());
          setData(a,3,job.created.value().asString("%d-%m-%y %h:%M:%s"));
          if(job.begintime!=1){
            setData(a,4,job.begintime.value().asString("%d-%m-%y %h:%M:%s"));
          }else{
            setData(a,4,std::string(""));
          }
          if(job.endtime!=1){
            setData(a,5,job.endtime.value().asString("%d-%m-%y %h:%M:%s"));
          }else{
            setData(a,5,std::string(""));
          }
          setData(a,6,job.progress.value());
          setData(a,7,job.status.value());
        }
      }

      JobTableModel::JobTableModel(const JobTableModel& orig) {
      }

      JobTableModel::~JobTableModel() {
      }
    }
  }
}

