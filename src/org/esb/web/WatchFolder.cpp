#include <Wt/Ext/ToolBar>
#include <Wt/Ext/Dialog>
#include <Wt/Ext/Container>
#include <Wt/WTree>
#include "FileTreeTable.h"
#include "FileTreeTableNode.h"
#include "SqlUtil.h"
#include "org/esb/config/config.h"
#include "org/esb/io/FileFilter.h"
#include "org/esb/io/File.h"

#include "WatchfolderForm.h"
#include "ColumnConfig.h"
#include "DbTable.h"
#include <boost/algorithm/string.hpp>

namespace org {
  namespace esb {
    namespace web {
      /*
      class DirectoryFileFilter : public org::esb::io::FileFilter {
      public:

      bool accept(org::esb::io::File file) {
      return file.isDirectory();
      }
      };
      */
      class WatchFolder : public Wt::Ext::Container {
      public:

        WatchFolder(int user_id) : Wt::Ext::Container() {
          using namespace org::esb::config;
          Wt::WFitLayout * l = new Wt::WFitLayout();
          setLayout(l);

          list<ColumnConfig> cc;
          cc.push_back(ColumnConfig(db::Watchfolder::Id,"Id",20));
          cc.push_back(ColumnConfig(db::Watchfolder::Infolder,"Input Folder",200));
          cc.push_back(ColumnConfig(db::Watchfolder::Outfolder,"Output Folder",200));

          tab=new DbTable(cc, litesql::Expr());
          tab->itemSelectionChanged().connect(SLOT(this, WatchFolder::enableEditButton));
          tab->setTopToolBar(new Wt::Ext::ToolBar());
          tab->doubleClicked.connect(SLOT(this, WatchFolder::editWatchFolder));
          layout()->addWidget(tab);


          //          tab->resize(Wt::WLength(), 300);

          buttonEdit = tab->topToolBar()->addButton("Edit selected Watch Folder");
          buttonEdit->clicked().connect(SLOT(this, WatchFolder::editWatchFolder));
          tab->topToolBar()->addSeparator();
          buttonDelete = tab->topToolBar()->addButton("Delete selected Watch Folder");
          buttonEdit->setEnabled(false);
          buttonDelete->setEnabled(false);
          /*          Wt::WGroupBox * group = new Wt::WGroupBox("WatchFolder", this);
          Wt::WTable *t = new Wt::WTable(group);

          buttonEdit->setEnabled(false);
          buttonNew->setEnabled(true);
          buttonEdit->clicked.connect(SLOT(this, WatchFolder::editWatchFolder));
          buttonNew->clicked.connect(SLOT(this, WatchFolder::newWatchFolder));

          int i = 0;
          buildElement("id", "Id", t, i++)->setEnabled(false);
          buildElement("infolder", "Input Watch Folder", t, i++)->setEnabled(false);
          selectInDirectory = new Wt::Ext::Button("Select Directory", t->elementAt(i - 1, 2));
          selectInDirectory->setEnabled(false);
          buildElement("outfolder", "Output Folder", t, i++)->setEnabled(false);
          selectOutDirectory = new Wt::Ext::Button("Select Directory", t->elementAt(i - 1, 2));
          selectOutDirectory->setEnabled(false);
          //          buildElement("profile", "Profile", t, i++)->setEnabled(false);


          //          profiles = new Wt::Ext::ComboBox(this);
          Wt::WLabel * elementLabel = new Wt::WLabel("Profile", t->elementAt(i, 0));
          t->elementAt(i, 0)->resize(Wt::WLength(14, Wt::WLength::FontEx), Wt::WLength());
          profiles = new Wt::Ext::ComboBox(t->elementAt(i, 1));
          profiles->setEnabled(false);
          Connection con(std::string(Config::getProperty("db.connection")));

          Statement stmt = con.createStatement("select * from profiles");
          ResultSet rs = stmt.executeQuery();
          int a=0;
          while (rs.next()) {
          profiles->addItem(rs.getString("profile_name"));
          name2id[rs.getString("profile_name")] = rs.getInt("id");
          id2name[rs.getInt("id")] = rs.getString("profile_name");
          profileid2profileidx[rs.getInt("id")]=a++;
          }
          elements["profile"] = profiles;
          i++;

          msg = new Wt::WText(t->elementAt(i, 0));
          buttonSave = new Wt::Ext::Button("Save", t->elementAt(i, 1));
          buttonSave->clicked.connect(SLOT(this, WatchFolder::saveMap));
          buttonSave->setEnabled(false);

          indirectoryChooser = new Wt::Ext::Dialog("Choose Directory");
          intree = new FileTreeTable(Config::getProperty("hive.scandir","/"),filter, indirectoryChooser->contents());
          intree->resize(500,300);
          Wt::Ext::Button *inselect = new Wt::Ext::Button("Select", indirectoryChooser->contents());
          inselect->clicked.connect(SLOT(indirectoryChooser, Wt::Ext::Dialog::accept));
          indirectoryChooser->resize(600, 400);
          intree->tree()->itemSelectionChanged.connect(SLOT(this,WatchFolder::selectInFolder));

          outdirectoryChooser = new Wt::Ext::Dialog("Choose Directory");
          //          outdirectoryChooser->setTitleBar(true);
          logdebug("hive.scandir:"<<Config::getProperty("hive.scandir","/"));
          outtree = new FileTreeTable(Config::getProperty("hive.scandir","/"),filter, outdirectoryChooser->contents());
          outtree->resize(500,300);
          Wt::Ext::Button *outselect = new Wt::Ext::Button("Select", outdirectoryChooser->contents());
          outselect->clicked.connect(SLOT(outdirectoryChooser, Wt::Ext::Dialog::accept));
          outdirectoryChooser->resize(600, 400);
          outtree->tree()->itemSelectionChanged.connect(SLOT(this,WatchFolder::selectOutFolder));

          selectInDirectory->clicked.connect(SLOT(this, WatchFolder::openInDirectoryChooser));
          selectOutDirectory->clicked.connect(SLOT(this, WatchFolder::openOutDirectoryChooser));
          tab->itemSelectionChanged.connect(SLOT(this, WatchFolder::enableEditButton));
          */
        }

       
      void createWatchFolder() {
          _db=boost::shared_ptr<db::HiveDb>(new db::HiveDb("mysql",org::esb::config::Config::getProperty("db.url")));
          _db->verbose=true;
          _wf=boost::shared_ptr<db::Watchfolder>(new db::Watchfolder(*_db.get()));
          WatchfolderForm * pf = new WatchfolderForm(*_wf.get());
          pf->saved.connect(SLOT(this, WatchFolder::folderSaved));
          pf->canceled.connect(SLOT(pf, Wt::Ext::Dialog::accept));

          pf->show();

      }
      private:
        Wt::Ext::Button * buttonEdit;
        Wt::Ext::Button * buttonDelete;
        DbTable * tab;

        boost::shared_ptr<db::Watchfolder> _wf;
        boost::shared_ptr<db::HiveDb> _db;

        void enableEditButton() {
          buttonEdit->setEnabled(true);
        }

        void editWatchFolder() {
          int id = atoi(boost::any_cast<string > (tab->getModel()->data(tab->selectedRows()[0], 0)).c_str());
          _db=boost::shared_ptr<db::HiveDb>(new db::HiveDb("mysql",org::esb::config::Config::getProperty("db.url")));
          
          _db->verbose=true;
          _wf=boost::shared_ptr<db::Watchfolder>(new db::Watchfolder(litesql::select<db::Watchfolder>(*_db.get(),db::Watchfolder::Id==id).one()));
          WatchfolderForm * pf = new WatchfolderForm(*_wf.get());
          pf->saved.connect(SLOT(this, WatchFolder::folderSaved));
          pf->canceled.connect(SLOT(pf, Wt::Ext::Dialog::accept));

          pf->show();
        }
        void folderSaved() {
          LOGDEBUG("save");
          _wf->update();
          tab->reload();          
        }
        /*
        void newWatchFolder() {
          std::map<std::string, Wt::Ext::LineEdit*>::iterator it = elements.begin();
          for (; it != elements.end(); it++) {
            (*it).second->setText("");
          }
          buttonSave->setEnabled(true);
          selectInDirectory->setEnabled(true);
          selectOutDirectory->setEnabled(true);
          elements["profile"]->setEnabled(true);
        }

        void selectInFolder() {

          Wt::WTree::WTreeNodeSet set = intree->tree()->selectedNodes();
          Wt::WTree::WTreeNodeSet::iterator it = set.begin();
          if (it != set.end()) {
            FileTreeTableNode * node = (FileTreeTableNode*) * it;
            elements["infolder"]->setText(node->path_.string());
          }
        }

        void selectOutFolder() {

          Wt::WTree::WTreeNodeSet set = outtree->tree()->selectedNodes();
          Wt::WTree::WTreeNodeSet::iterator it = set.begin();
          if (it != set.end()) {
            FileTreeTableNode * node = (FileTreeTableNode*) * it;
            elements["outfolder"]->setText(node->path_.string());
          }
        }*/

      };
    }
  }
}
