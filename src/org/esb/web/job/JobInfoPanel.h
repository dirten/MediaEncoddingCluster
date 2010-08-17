/* 
 * File:   JobInfoPanel.h
 * Author: HoelscJ
 *
 * Created on 17. August 2010, 14:43
 */

#ifndef JOBINFOPANEL_H
#define	JOBINFOPANEL_H
#include "Wt/Ext/TabWidget"
#include "JobDetailPanel.h"
#include "JobLogPanel.h"
#include "org/esb/lang/Ptr.h"
namespace org {
  namespace esb {
    namespace web {

      class JobInfoPanel:public Wt::Ext::TabWidget {
      public:
        JobInfoPanel();
        JobInfoPanel(const JobInfoPanel& orig);
        void setJob(int jobid);
        virtual ~JobInfoPanel();
      private:
        Ptr<JobDetailPanel>_detail_panel;
        Ptr<JobLogPanel>_log_panel;
      };
    }
  }
}

#endif	/* JOBINFOPANEL_H */

