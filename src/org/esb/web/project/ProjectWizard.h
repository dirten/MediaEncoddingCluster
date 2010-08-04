/* 
 * File:   ProjectWizard.h
 * Author: HoelscJ
 *
 * Created on 13. Juli 2010, 14:10
 */

#ifndef PROJECTWIZARD_H
#define	PROJECTWIZARD_H
#include "org/esb/db/hivedb.hpp"
#include "Wt/Ext/Dialog"
#include "InputFilePanel.h"
#include "ProfilePanel.h"
#include "FilterPanel.h"
#include "PreviewPanel.h"
#include "ProjectPropertyPanel.h"
#include "org/esb/lang/Ptr.h"
namespace org{
  namespace esb{
    namespace web{
      class ProjectWizard : public Wt::Ext::Dialog{
      public:
        ProjectWizard();
        void open();
        void open(int pid);
        void open(Ptr<db::Project>);
        void save();
        void save_and_start();
        void cancel();
        Wt::Signal<void> saved;
        Wt::Signal<void> canceled;
      private:
        int _project_id;
        Ptr<db::Project> _project;
        Ptr<InputFilePanel> _filePanel;
        Ptr<ProfilePanel> _profilePanel;
        Ptr<FilterPanel> _filterPanel;
        Ptr<PreviewPanel> _previewPanel;
        Ptr<ProjectPropertyPanel> _propertyPanel;
        Ptr<db::HiveDb> _db;
      };
    }
  }
}


#endif	/* PROJECTWIZARD_H */

