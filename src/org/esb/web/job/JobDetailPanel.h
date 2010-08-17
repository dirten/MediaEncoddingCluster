/* 
 * File:   JobDetailPanel.h
 * Author: HoelscJ
 *
 * Created on 17. August 2010, 14:46
 */

#ifndef JOBDETAILPANEL_H
#define	JOBDETAILPANEL_H
#include "org/esb/db/hivedb.hpp"
#include "Wt/Ext/Panel"

namespace org {
  namespace esb {
    namespace web {

      class JobDetailPanel:public Wt::Ext::Panel {
      public:
        JobDetailPanel();
        JobDetailPanel(const JobDetailPanel& orig);
        virtual ~JobDetailPanel();
        void setJob(db::Job job);
      private:

      };
    }
  }
}
#endif	/* JOBDETAILPANEL_H */

