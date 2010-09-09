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
#include <Wt/WFitLayout>

namespace org {
  namespace esb {
    namespace web {

      FileChooser::FileChooser(std::string title, std::string path) : Wt::Ext::Dialog(title) {
        resize(400, 400);
        //setLayout(new Wt::WFitLayout());
        tree = new FileTreeTable("/", filter, contents());
//        tree->resize(380, 300);
        setAutoScrollBars(true);
        addButton(new Wt::Ext::Button("Refresh"));
        buttons().back()->clicked.connect(SLOT(this, FileChooser::ownrefresh));
        addButton(new Wt::Ext::Button("Select File"));
        buttons().back()->clicked.connect(SLOT(this, FileChooser::accept));

        addButton(new Wt::Ext::Button("Cancel"));
        buttons().back()->clicked.connect(SLOT(this, FileChooser::cancel));

      }
      FileChooser::~FileChooser(){
        LOGDEBUG("FileChooser::~FileChooser()");
        //delete tree;
      }

      void FileChooser::ownrefresh() {
        delete tree;
        tree = new FileTreeTable("/", filter, contents());
        this->refresh();
      }
      void FileChooser::accept() {
        LOGDEBUG("accept");
        Wt::Ext::Dialog::accept();
      }
      
      std::list<Ptr<org::esb::io::File> > FileChooser::getSelectedFiles() {
        std::list<Ptr<org::esb::io::File> > result;
        Wt::WTree::WTreeNodeSet set = tree->tree()->selectedNodes();
        Wt::WTree::WTreeNodeSet::iterator it = set.begin();
        for(;it != set.end();it++) {
          FileTreeTableNode * node = (FileTreeTableNode*) * it;
          LOGDEBUG(node->path_.string());
          result.push_back(new org::esb::io::File(node->path_.string()));
        }
        return result;
      }

      void FileChooser::select() {
        std::list<Ptr<org::esb::io::File> > result=getSelectedFiles();
        selected.emit(result);
        LOGDEBUG("after emit");
      }

      void FileChooser::cancel() {
        Wt::Ext::Dialog::reject();
//        canceled.emit();
      }
    }
  }
}

