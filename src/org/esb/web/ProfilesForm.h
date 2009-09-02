#include <Wt/WContainerWidget>
#include "wtk/ElementContainer.h"

#include <Wt/Ext/LineEdit>
#include <Wt/Ext/ComboBox>
#include <map>
#ifndef _PROFILESFORM_H
#define	_PROFILESFORM_H
namespace org {
    namespace esb {
        namespace web {

            class ProfilesForm : public Wt::WContainerWidget {
            public:
                ProfilesForm(Wt::WContainerWidget *);
                void setProfile(int profileid);
                //				void saveProfile();
                Wt::Signal<void> profileSaved;
                Wt::Signal<void> profileCanceled;
            private:
                void cancelProfile();
                void saveProfile();
                wtk::ElementContainer<Wt::Ext::LineEdit> _el;
                wtk::ElementContainer<Wt::Ext::ComboBox> _elcb;

                std::map<int, std::string> formatidx2formatname;
                std::map<std::string, int> formatname2formatidx;

                std::map<int, std::string> codecid2codecname;
                std::map<std::string, int> codecname2codecid;

                std::map<int, int> acodecid2acodecidx;
                std::map<int, int> vcodecid2vcodecidx;
                std::map<int, int> formatid2formatidx;


            };

        }
    }
}
#endif
