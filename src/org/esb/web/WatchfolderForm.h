/* 
 * File:   WatchfolderForm.h
 * Author: jhoelscher
 *
 * Created on 2. September 2009, 14:55
 */

#include <Wt/WContainerWidget>
#include <Wt/Ext/LineEdit>
#include <Wt/Ext/ComboBox>
#include "wtk/ElementContainer.h"
#include "DirectoryChooser.h"
#include <map>

#ifndef _WATCHFOLDERFORM_H
#define	_WATCHFOLDERFORM_H
namespace org {
    namespace esb {
        namespace web {

            class WatchfolderForm : public Wt::WContainerWidget {
            public:
                WatchfolderForm(Wt::WContainerWidget *);
                void setWatchfolder(int);
                Wt::Signal<void> saved;
                Wt::Signal<void> canceled;
            private:
                void save();
                void cancel();
                void openInfolder();
                void selectedInfolder(std::string);
                void openOutfolder();
                void selectedOutfolder(std::string);
                wtk::ElementContainer<Wt::Ext::LineEdit> _le;
                wtk::ElementContainer<Wt::Ext::ComboBox> _cb;
                std::map<std::string, int> name2id;
                std::map<int, std::string> id2name;
                std::map<int, int> profileid2profileidx;
                DirectoryChooser * dc;
            };

        }
    }
}


#endif	/* _WATCHFOLDERFORM_H */

