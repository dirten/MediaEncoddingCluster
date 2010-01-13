#include "DirectoryChooser.h"

#include "FileTreeTable.h"
#include "FileTreeTableNode.h"
#include <Wt/WTable>
#include <Wt/WBreak>
#include <Wt/Ext/Button>
#include <Wt/WTree>

namespace org {
  namespace esb {
    namespace web {

      DirectoryChooser::DirectoryChooser(std::string title, std::string path) : Wt::Ext::Dialog(title) {
        resize(400, 400);

        tree = new FileTreeTable("/", filter, contents());
        tree->resize(380, 300);
        new Wt::WBreak(contents());
        Wt::WTable * b = new Wt::WTable(contents());
        Wt::Ext::Button *cancel = new Wt::Ext::Button("Cancel", b->elementAt(0, 0));
        Wt::Ext::Button *save = new Wt::Ext::Button("Select Directory", b->elementAt(0, 1));
        cancel->clicked.connect(SLOT(this, DirectoryChooser::cancel));
        save->clicked.connect(SLOT(this, DirectoryChooser::select));


      }

      void DirectoryChooser::select() {
        Wt::WTree::WTreeNodeSet set = tree->tree()->selectedNodes();
        Wt::WTree::WTreeNodeSet::iterator it = set.begin();
        std::string path;
        if (it != set.end()) {
//          logdebug("Folder in set");
          FileTreeTableNode * node = (FileTreeTableNode*) * it;
          path=node->path_.string();
//          logdebug(node->path_);
        }

        selected.emit(path);
      }

      void DirectoryChooser::cancel() {
        canceled.emit();
      }
    }
  }
}

