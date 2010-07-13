/* 
 * File:   ProjectWizard.h
 * Author: HoelscJ
 *
 * Created on 13. Juli 2010, 14:10
 */

#ifndef PROJECTWIZARD_H
#define	PROJECTWIZARD_H

#include "Wt/Ext/Dialog"

namespace org{
  namespace esb{
    namespace web{
      class ProjectWizard : public Wt::Ext::Dialog{
      public:
        ProjectWizard();
        void open();
        void save();
        void cancel();
      private:
        int _project_id;
      };
    }
  }
}


#endif	/* PROJECTWIZARD_H */

