#ifndef ORG_ESB_WEB_PROJECTS
#define ORG_ESB_WEB_PROJECTS

#include <Wt/Ext/Container>
#include <Wt/Ext/Button>
#include "org/esb/lang/Ptr.h"
#include "ProjectTable.h"
namespace org{
  namespace esb{
    namespace web{
      class DbTable;
      class ProjectWizard;
      class Projects: public Wt::Ext::Container {
      public:
        Projects(Wt::WContainerWidget * parent = 0) ;
        void createProject();
      private:
        Ptr<ProjectTable> _table;
        void editProject();
        void deleteProject();
        void enableButtons();
        void projectSaved();
        Wt::Ext::Button * create_button;
        Wt::Ext::Button * edit_button;
        Wt::Ext::Button * delete_button;
        Ptr<ProjectWizard> _wizard;
      };
    }
  }
}
#endif
