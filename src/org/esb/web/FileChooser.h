/* 
 * File:   FileChooser.h
 * Author: HoelscJ
 *
 * Created on 13. Juli 2010, 17:37
 */

#ifndef FILECHOOSER_H
#define	FILECHOOSER_H
#include <Wt/Ext/Dialog>
#include "FileTreeTable.h"

#include "org/esb/io/FileFilter.h"
#include "org/esb/io/File.h"
namespace org {
  namespace esb {
    namespace web {


            class FileChooserFilter : public org::esb::io::FileFilter {
            public:

                bool accept(org::esb::io::File & file) {
                  return file.isDirectory()||file.isFile();
                }
            };

            class FileChooser : public Wt::Ext::Dialog {
            public:
                FileChooser(std::string title, std::string path = "");
                Wt::Signal<org::esb::io::File> selected;
                Wt::Signal<void> canceled;
                ~FileChooser();
            private:
                void select();
                void cancel();
//                org::esb::io::File _path;
                FileChooserFilter filter;
                FileTreeTable * tree;

            };
    }
  }
}

#endif	/* FILECHOOSER_H */

