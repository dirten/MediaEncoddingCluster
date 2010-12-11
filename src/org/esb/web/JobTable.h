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
#include "Wt/WTimer"
#include "Wt/Ext/ComboBox"

#include "org/esb/util/Log.h"
namespace org {
  namespace esb {
    namespace web {
      class JobTableModel;
      class JobTable:public Wt::Ext::TableView {
        classlogger("org.esb.web.JobTable");
      public:
        JobTable();
        virtual ~JobTable();
        void refresh();
        void /*Wt::JSignal<std::string,int>&*/ stopEncoding(int);
      private:
        Ptr<JobTableModel> _model;
        Wt::WTimer *timer;
        void rowSelected();
        Wt::JSignal<int> _stopEncoding;
        void refreshTimerChanged();
        Wt::Ext::ComboBox * _timerBox;
      };

    }
  }
}

#endif	/* JOBTABLE_H */

