/* 
 * File:   JobTable.cpp
 * Author: HoelscJ
 * 
 * Created on 13. August 2010, 17:32
 */

#include "org/esb/db/hivedb.hpp"
#include "Wt/Ext/MessageBox"
#include "Wt/Ext/ToolBar"

#include "JobTable.h"
#include "JobTableModel.h"

#include "org/esb/config/config.h"
#include "Wt/WCompositeWidget"
#include "org/esb/hive/DatabaseService.h"
#include "org/esb/util/StringUtil.h"
#include "org/esb/signal/Messenger.h"
#include "org/esb/signal/Message.h"
/*on windows there is a macro defined with name MessageBox*/
#ifdef MessageBox
#undef MessageBox
#endif
namespace org {
  namespace esb {
    namespace web {

      JobTable::JobTable() : Wt::Ext::TableView(),_stopEncoding(this, "stopEncoding") {
        LOGDEBUG("JobTable::JobTable()");
        db::HiveDb dbCon=org::esb::hive::DatabaseService::getDatabase();
        
        std::vector<db::Job> jobs = litesql::select<db::Job > (dbCon).orderBy(db::Job::Id, false).all();
        _model=Ptr<JobTableModel>(new JobTableModel(jobs));
        setModel(_model.get());
        setAlternatingRowColors(true);
        setColumnSortable(1,true);
        setColumnSortable(2,true);
        setColumnSortable(3,true);
        setColumnSortable(4,true);
        setColumnSortable(5,true);
        setColumnSortable(6,true);
        setColumnSortable(7,true);
        setColumnSortable(8,true);
        setColumnSortable(9,true);
        setColumnSortable(10,true);
        setColumnSortable(11,true);

        resizeColumnsToContents(true);
        setHighlightMouseOver(true);
        setSelectionBehavior(Wt::SelectRows);
        setSelectionMode(Wt::SingleSelection);
        setColumnHidden(0, true);
        setColumnWidth(1, 10);
        
        setColumnWidth(5, 35);
        setColumnWidth(6, 35);
        setColumnWidth(7, 35);
        setColumnWidth(8, 25);
        setColumnWidth(9, 25);
        setColumnWidth(10, 15);

        std::string renderer = "function change(val) {"
                "if (val > 0){"
                "return '<span style=\"color:green;\">' + val + '</span>';"
                "} else if(val < 0) {"
                "return '<span style=\"color:red;\">' + val + '</span>';"
                "}"
                "return val;"
                "}";
        setRenderer(9, renderer);
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
                "} else if(val == \"exists\") {"
                "return '<img src=\"/icons/warning-icon.png\"/>';"
                "}"
                "return val;"
                "}";
        setRenderer(10, renderer);
        
//        std::string element=doSome_.createCall("'test'","42");
//        LOGDEBUG("Element:"<<element);
        _stopEncoding.connect(this, &JobTable::stopEncoding);
        //std::string element=doSome_.createCall("'test'","42");
        //element.replace("\'","\\\'");
        //LOGDEBUG("Element:"<<element);

        renderer = "function change(val) {"
//                "return \"<a href=\\\"javascript:Wt.emit('"+id()+"','"+doSome_.name()+"', \"+val+\");\\\"><img src=\\\"/icons/pause.png\\\"/></a><a href=\\\"javascript:Wt.emit('"+id()+"','"+doSome_.name()+"', \"+val+\");\\\"><img src=\\\"/icons/up.png\\\"/></a><a href=\\\"javascript:Wt.emit('"+id()+"','"+doSome_.name()+"', \"+val+\");\\\"><img src=\\\"/icons/down.png\\\"/></a>\""
                "return \"<a href=\\\"javascript:Wt.emit('"+id()+"','"+_stopEncoding.name()+"', \"+val+\");\\\"><img src=\\\"/icons/delete-icon.png\\\" alt=\\\"stop Encoding\\\nit can not be restarted!!! \\\"/></a>\""
                "}";
        setRenderer(1, renderer);
      setTopToolBar(new Wt::Ext::ToolBar());
      _timerBox=new Wt::Ext::ComboBox();
      _timerBox->addItem("no auto refresh");
      _timerBox->addItem("5 sec.");
      _timerBox->addItem("10 sec.");
      _timerBox->addItem("30 sec.");
      _timerBox->addItem("60 sec.");
      _timerBox->addItem("120 sec.");
      _timerBox->setCurrentIndex(0);
      _timerBox->activated().connect(SLOT(this,JobTable::refreshTimerChanged));
      topToolBar()->add(_timerBox);
      timer = new Wt::WTimer();
      //timer->setInterval(2000);
      timer->timeout().connect(SLOT(this, JobTable::refresh));
      //timer->start();
      
      }
      /*
      void JobTable::rowSelected(){

      }*/
      void JobTable::stopEncoding(int t) {
        using namespace org::esb::signal;

        LOGDEBUG("Wt::JSignal<std::string, int>& JobTable::stopEncoding()"<<t);
        db::HiveDb dbCon=org::esb::hive::DatabaseService::getDatabase();
        if(litesql::select<db::Job > (dbCon,db::Job::Id==t).count()>0){
          db::Job job = litesql::select<db::Job > (dbCon,db::Job::Id==t).one();
          Wt::Ext::MessageBox *box = new Wt::Ext::MessageBox("Stopping the Encoding Job", "Do you really want to stop this Job?", Wt::Warning, Wt::Ok|Wt::Cancel);
          box->show();

          if(box->exec()==Wt::Ext::Dialog::Accepted&&job.status!="stopped"){
            job.status=job.status=="queued"?"stopped":"stopping";
            job.update();
            std::string job_id=org::esb::util::StringUtil::toString(t);
            Messenger::getInstance().sendMessage(Message().setProperty("processunitcontroller","STOP_JOB").setProperty("job_id",job_id));
          }
          delete box;
        }
      }

      void JobTable::refreshTimerChanged() {
        std::string sec=_timerBox->currentText().narrow();
        int isec=atoi(sec.c_str());
        LOGINFO("Setting refreshTimer to"<<isec);
        timer->stop();
        delete timer;
        timer=NULL;
        if(isec>0){
          isec*=1000;
          timer = new Wt::WTimer();
          timer->timeout().connect(SLOT(this, JobTable::refresh));
          timer->setInterval(isec);
          timer->start();
        }
      }

      void JobTable::refresh() {
        LOGDEBUG("JobTable::refresh()")
        db::HiveDb dbCon=org::esb::hive::DatabaseService::getDatabase();

        std::vector<db::Job> jobs = litesql::select<db::Job > (dbCon).all();
        _model->refresh(jobs);
      }


      JobTable::~JobTable() {
        LOGDEBUG("JobTable::~JobTable()");
        delete timer;
        timer=NULL;
      }

    }
  }
}