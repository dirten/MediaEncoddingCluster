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
#include <Wt/WBorderLayout>

namespace org {
  namespace esb {
    namespace web {

      FileChooser::FileChooser(std::string title, std::string path) : Wt::Ext::Dialog(title) {
        contents()->setLayout(new Wt::WBorderLayout());
        //setLayout(new Wt::WFitLayout());
        //this->setMargin(0);
        tree = new FileTreeTable("/", filter);
        /*
        Wt::Ext::Panel * comp=new Wt::Ext::Panel();
        Wt::Ext::Panel * view=new Wt::Ext::Panel();
        //view->setLayout(new Wt::WBorderLayout());
        view->setLayout(new Wt::WFitLayout());
        view->setTitle("Files");
        view->resize(Wt::WLength::Auto,Wt::WLength::Auto);
        view->layout()->addWidget(tree);
        //view->layout()->addWidget(new Wt::Ext::Button("testbutton"));
        view->setAutoScrollBars(true);
        //view->setLayoutSizeAware(true);

        comp->setTitle("Sources");
        comp->layout()->addWidget(new Wt::Ext::Button("File Source"));
        comp->layout()->addWidget(new Wt::Ext::Button("HTTP Source"));
        comp->layout()->addWidget(new Wt::Ext::Button("Upload Source"));
        */
        static_cast<Wt::WBorderLayout*>(contents()->layout())->setSpacing(3);
        static_cast<Wt::WBorderLayout*>(contents()->layout())->setContentsMargins(0,0,0,0);
        //static_cast<Wt::WBorderLayout*>(contents()->layout())->addWidget(comp, Wt::WBorderLayout::West);
        static_cast<Wt::WBorderLayout*>(contents()->layout())->addWidget(tree, Wt::WBorderLayout::Center);

//        tree->resize(380, 300);
        resize(600, 400);
        //setAutoScrollBars(true);
        //addButton(new Wt::Ext::Button("Refresh"));
        //buttons().back()->clicked().connect(SLOT(this, FileChooser::ownrefresh));
        addButton(new Wt::Ext::Button("Select File"));
        buttons().back()->clicked().connect(SLOT(this, FileChooser::accept));

        addButton(new Wt::Ext::Button("Cancel"));
        buttons().back()->clicked().connect(SLOT(this, FileChooser::cancel));

      }
      FileChooser::~FileChooser(){
        LOGDEBUG("FileChooser::~FileChooser()");
        //delete tree;
      }

      void FileChooser::ownrefresh() {
        Wt::WLayoutItem * item=static_cast<Wt::WBorderLayout*>(contents()->layout())->itemAt(Wt::WBorderLayout::Center);
        static_cast<Wt::WBorderLayout*>(contents()->layout())->removeItem(item);
        delete item;
        static_cast<Wt::WBorderLayout*>(contents()->layout())->addWidget(new FileTreeTable("/", filter), Wt::WBorderLayout::Center);
        //tree = new FileTreeTable("/", filter);
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

