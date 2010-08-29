/* 
 * File:   JobTable.cpp
 * Author: HoelscJ
 * 
 * Created on 13. August 2010, 17:32
 */

#include "org/esb/db/hivedb.hpp"
#include "JobTable.h"
#include "JobTableModel.h"

#include "org/esb/config/config.h"
#include "Wt/WTimer"
namespace org {
  namespace esb {
    namespace web {

      JobTable::JobTable() : Wt::Ext::TableView() {
        LOGDEBUG("JobTable::JobTable()");
        db::HiveDb dbCon("mysql", org::esb::config::Config::getProperty("db.url"));
        
        std::vector<db::Job> jobs = litesql::select<db::Job > (dbCon).orderBy(db::Job::Id, false).all();
        _model=Ptr<JobTableModel>(new JobTableModel(jobs));
        setModel(_model.get());
        setAlternatingRowColors(true);
        resizeColumnsToContents(true);
        setHighlightMouseOver(true);
        setSelectionBehavior(Wt::SelectRows);
        setSelectionMode(Wt::SingleSelection);
        setColumnWidth(0, 3);
        setColumnWidth(3, 30);
        setColumnWidth(4, 30);
        setColumnWidth(5, 30);
        setColumnWidth(6, 25);
        setColumnWidth(7, 15);

        std::string renderer = "function change(val) {"
                "if (val > 0){"
                "return '<span style=\"color:green;\">' + val + '</span>';"
                "} else if(val < 0) {"
                "return '<span style=\"color:red;\">' + val + '</span>';"
                "}"
                "return val;"
                "}";
        setRenderer(6, renderer);
        renderer = "function change(val) {"
                "if (val == \"running\"){"
                "return '<img src=\"/icons/encoding-in-progress.gif\"/>';"
                "} else if(val == \"failed\") {"
                "return '<img src=\"/icons/remove-icon.png\"/>';"
                "} else if(val == \"exported\") {"
                "return '<img src=\"/icons/accept-icon.png\"/>';"
                "} else if(val == \"queued\") {"
                "return '<img src=\"/icons/queued-icon.png\"/>';"
                "} else if(val == \"warning\") {"
                "return '<img src=\"/icons/warning-icon.png\"/>';"
                "}"
                "return val;"
                "}";
        setRenderer(7, renderer);
       Wt::WTimer *timer = new Wt::WTimer();
      timer->setInterval(2000);
      timer->timeout.connect(SLOT(this, JobTable::refresh));
      timer->start();
      }

      void JobTable::refresh() {
        LOGDEBUG("JobTable::refresh()")
        db::HiveDb dbCon("mysql", org::esb::config::Config::getProperty("db.url"));

        std::vector<db::Job> jobs = litesql::select<db::Job > (dbCon).orderBy(db::Job::Id, false).all();
        _model->refresh(jobs);
      }

      JobTable::JobTable(const JobTable& orig) {
        LOGDEBUG("JobTable::JobTable(const JobTable& orig)")
      }

      JobTable::~JobTable() {
        LOGDEBUG("JobTable::~JobTable()");
      }

    }
  }
}