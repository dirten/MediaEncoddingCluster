/* 
 * File:   WatchfolderForm.h
 * Author: jhoelscher
 *
 * Created on 2. September 2009, 14:55
 */
#include "org/esb/db/hivedb.hpp"
#include <Wt/WContainerWidget>
#include <Wt/Ext/LineEdit>
#include <Wt/Ext/ComboBox>
#include <Wt/Ext/Dialog>
#include "wtk/ElementContainer.h"
#include "DirectoryChooser.h"
#include <map>

#ifndef _WATCHFOLDERFORM_H
#define	_WATCHFOLDERFORM_H
namespace org {
    namespace esb {
        namespace web {

            class WatchfolderForm : public Wt::Ext::Dialog{
            public:
                WatchfolderForm(db::Watchfolder & folder);
                void setWatchfolder(int);
                Wt::Signal<void> saved;
                Wt::Signal<void> canceled;
                void setWatchfolder(db::Watchfolder & folder);
            private:
                void save();
                void cancel();
                void changed();
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
                db::Watchfolder & _folder;
            };

        }
    }
}


#endif	/* _WATCHFOLDERFORM_H */

