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
        insertColumns(0, 9);
        int i=0;
        setHeaderData(i++, std::string("Id"));
        setHeaderData(i++, std::string("Stop"));
        setHeaderData(i++, std::string("Input File"));
        setHeaderData(i++, std::string("Output File"));
        setHeaderData(i++, std::string("Enqueue Time"));
        setHeaderData(i++, std::string("Start Time"));
        setHeaderData(i++, std::string("End Time"));
        setHeaderData(i++, std::string("Progress %"));
        setHeaderData(i++, std::string("Status"));
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
          int col=0;
          setData(a,col++,org::esb::util::StringUtil::toString(job.id.value()));
          setData(a,col++,org::esb::util::StringUtil::toString(job.id.value()));
          setData(a,col++,""+job.infile.value()+"");
          setData(a,col++,job.outfile.value());
          setData(a,col++,job.created.value().asString("%d-%m-%y %h:%M:%s"));
          if(job.begintime!=1){
            setData(a,col++,job.begintime.value().asString("%d-%m-%y %h:%M:%s"));
          }else{
            setData(a,col++,std::string(""));
          }
          if(job.endtime!=1){
            setData(a,col++,job.endtime.value().asString("%d-%m-%y %h:%M:%s"));
          }else{
            setData(a,col++,std::string(""));
          }
          setData(a,col++,job.progress.value());
          setData(a,col++,job.status.value());
        }
      }

      JobTableModel::JobTableModel(const JobTableModel& orig) {
      }

      JobTableModel::~JobTableModel() {
      }
    }
  }
}

