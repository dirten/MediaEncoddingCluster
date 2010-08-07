

#ifndef ORG_ESB_WEB_PROFILECHOOSER
#define ORG_ESB_WEB_PROFILECHOOSER
#include "org/esb/db/hivedb.hpp"
#include "Wt/Ext/Dialog"
#include "Wt/WSignal"
#include "org/esb/lang/Ptr.h"

namespace org{
  namespace esb{
    namespace web{
      class ProfileTable;
      class ProfileChooser: public Wt::Ext::Dialog{
      public:
        ProfileChooser();
        ~ProfileChooser();
        int getSelectedProfileId();
        Wt::Signal<void> selected;
        Wt::Signal<void> canceled;
      private:
        void choose();
        void cancel();
        int selected_profile_id;
        Ptr<ProfileTable> _profile_table;

      };
    }
  }
}



#endif
