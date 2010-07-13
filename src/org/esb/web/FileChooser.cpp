/* 
 * File:   FileChooser.cpp
 * Author: HoelscJ
 * 
 * Created on 13. Juli 2010, 17:37
 */

#include "FileChooser.h"

#include "FileTreeTable.h"
#include "FileTreeTableNode.h"
#include <Wt/WTable>
#include <Wt/WBreak>
#include <Wt/Ext/Button>
#include <Wt/WTree>

namespace org {
  namespace esb {
    namespace web {

      FileChooser::FileChooser(std::string title, std::string path) : Wt::Ext::Dialog(title) {
        resize(400, 400);

        tree = new FileTreeTable("/", filter, contents());
        tree->resize(380, 300);
        new Wt::WBreak(contents());
        Wt::WTable * b = new Wt::WTable(contents());
        Wt::Ext::Button *cancel = new Wt::Ext::Button("Cancel", b->elementAt(0, 0));
        Wt::Ext::Button *save = new Wt::Ext::Button("Select Directory", b->elementAt(0, 1));
        cancel->clicked.connect(SLOT(this, FileChooser::cancel));
        save->clicked.connect(SLOT(this, FileChooser::select));


      }

      void FileChooser::select() {
        Wt::WTree::WTreeNodeSet set = tree->tree()->selectedNodes();
        Wt::WTree::WTreeNodeSet::iterator it = set.begin();

        if (it != set.end()) {
          //          logdebug("Folder in set");
          FileTreeTableNode * node = (FileTreeTableNode*) * it;
          org::esb::io::File path = org::esb::io::File(node->path_.string());
          selected.emit(path);
          this->accept();
          //          logdebug(node->path_);
        }

      }

      void FileChooser::cancel() {
        Wt::Ext::Dialog::reject();
//        canceled.emit();
      }
    }
  }
}

