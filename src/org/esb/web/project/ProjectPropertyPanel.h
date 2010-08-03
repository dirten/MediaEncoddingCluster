#ifndef ORG_ESB_WEB_PROJECTPROPERTYPANEL
#define ORG_ESB_WEB_PROJECTPROPERTYPANEL

#include "org/esb/db/hivedb.hpp"
#include "Wt/Ext/Panel"
#include "org/esb/lang/Ptr.h"
#include "../DirectoryChooser.h"

#include "wtk/ElementContainer.h"
namespace org{
  namespace esb{
    namespace web{

      class ProjectPropertyPanel: public Wt::Ext::Panel{
      public:
        ProjectPropertyPanel();
        ~ProjectPropertyPanel();
        void setProject(Ptr<db::Project> p);
        void save();
      private:
        wtk::ElementContainer<Wt::Ext::LineEdit> _el;
        Ptr<db::Project> _project;
        Ptr<DirectoryChooser> _dc;
        void openOutfolder();
        void selectedOutfolder(std::string path);
      };
    }
  }
}

#endif

