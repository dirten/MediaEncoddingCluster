/* 
 * File:   DirectoryChooser.h
 * Author: jhoelscher
 *
 * Created on 2. September 2009, 16:41
 */

#include <Wt/Ext/Dialog>
#include "FileTreeTable.h"

#include "org/esb/io/FileFilter.h"
#include "org/esb/io/File.h"

#ifndef _DIRECTORYCHOOSER_H
#define	_DIRECTORYCHOOSER_H

namespace org {
    namespace esb {
        namespace web {

            class DirectoryFileFilter : public org::esb::io::FileFilter {
            public:

                bool accept(org::esb::io::File file) {
                    return file.isDirectory();
                }
            };

            class DirectoryChooser : public Wt::Ext::Dialog {
            public:
                DirectoryChooser(std::string title, std::string path = "");
                Wt::Signal<std::string> selected;
                Wt::Signal<void> canceled;
            private:
                void select();
                void cancel();
                std::string _path;
                DirectoryFileFilter filter;
                FileTreeTable * tree;

            };
        }
    }
}

#endif	/* _DIRECTORYCHOOSER_H */

