
#ifndef ORG_ESB_WEB_PROFILEPANEL
#define ORG_ESB_WEB_PROFILEPANEL

#include "org/esb/db/hivedb.hpp"
#include "Wt/Ext/Panel"
#include "org/esb/lang/Ptr.h"
#include "org/esb/util/Log.h"
#include "org/esb/web/presets/PresetsEditorWindow.h"
namespace org{
  namespace esb{
    namespace web{
      class ProfileTable;
      class ProfileCreator;
      class ProfileChooser;
      class ProfilePanel: public Wt::Ext::Panel{
        classlogger("org.esb.web.project.ProfilePanel")
      public:
        ProfilePanel();
        void setProject(Ptr<db::Project>);
      private:
        Ptr<ProfileTable> _profile_table;
        Ptr<PresetsEditorWindow> _profile_editor;
        Ptr<ProfileChooser> _profile_chooser;

        Ptr<db::Project> _project;
        Ptr<db::Profile> _profile;
        Wt::Ext::Button * removeProfileButton;
        Wt::Ext::Button * editProfileButton;
        void addProfileButtonClicked();
        void profileChooserSelected();
        void profileSelected();
        void removeSelectedProfile();
        void editSelectedProfile();
        void profileSaved();
        void enableButtons();
      };
    }
  }
}
#endif
