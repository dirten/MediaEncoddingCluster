/*
 * File:   JobInfoPanel.cpp
 * Author: HoelscJ
 * 
 * Created on 17. August 2010, 14:42
 */

#include "JobInfoPanel.h"
#include "org/esb/db/hivedb.hpp"
#include "org/esb/lang/Ptr.h"
#include "org/esb/config/config.h"
namespace org {
  namespace esb {
    namespace web {

      JobInfoPanel::JobInfoPanel():Wt::Ext::TabWidget(NULL) {
        //this->addTab((_detail_panel=new JobDetailPanel()).get(), "Job Details");
        this->addTab((_log_panel=new JobLogPanel()).get(), "Job Logging");
      }

      JobInfoPanel::JobInfoPanel(const JobInfoPanel& orig) {

      }

      JobInfoPanel::~JobInfoPanel() {

      }
      void JobInfoPanel::setJob(int id) {
        db::HiveDb dbCon("mysql", org::esb::config::Config::getProperty("db.url"));
        db::Job job=litesql::select<db::Job>(dbCon, db::Job::Id==id).one();
        //_detail_panel->setJob(job);
        _log_panel->setJob(job);
      }
    }
  }
}
