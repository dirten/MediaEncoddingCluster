/* 
 * File:   JobLogPanel.h
 * Author: HoelscJ
 *
 * Created on 17. August 2010, 15:33
 */

#ifndef JOBLOGPANEL_H
#define	JOBLOGPANEL_H
#include "org/esb/db/hivedb.hpp"
#include "Wt/Ext/TableView"
namespace org {
  namespace esb {
    namespace web {

      class JobLogPanel :public Wt::Ext::TableView{
      public:
        JobLogPanel();
        JobLogPanel(const JobLogPanel& orig);
        virtual ~JobLogPanel();
        void setJob(db::Job);
        void refresh();
      private:

      };
    }
  }
}
#endif	/* JOBLOGPANEL_H */

