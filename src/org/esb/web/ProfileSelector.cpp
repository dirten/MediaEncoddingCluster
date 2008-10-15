#ifndef ORG_ESB_WEB_PROFILESELECTOR
#define ORG_ESB_WEB_PROFILESELECTOR

#include "Wt/WContainerWidget"
namespace org{
    namespace esb{
        namespace web{
            class ProfileSelector: public Wt::WContainerWidget{
            public:
                ProfileSelector(Wt::WContainerWidget*parent=0):Wt::WContainerWidget(parent){
                    
                }
            };
        }
    }
}
#endif


