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
          _user_id = user_id;
          tab = new SqlTable(std::string("SELECT watch_folder.id,infolder, outfolder, extension_filter, profile_name from watch_folder, profiles where profiles.id=profile"));
          tab->itemSelectionChanged.connect(SLOT(this, WatchFolder::enableEditButton));
          tab->setTopToolBar(new Wt::Ext::ToolBar());

          layout()->addWidget(tab);


          //          tab->resize(Wt::WLength(), 300);

          buttonEdit = tab->topToolBar()->addButton("Edit selected Watch Folder");
          buttonEdit->clicked.connect(SLOT(this, WatchFolder::editWatchFolder));
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

        ~WatchFolder() {

        }

      private:
        Wt::Ext::Button * buttonEdit;
        Wt::Ext::Button * buttonNew;
        Wt::Ext::Button * buttonDelete;
        Wt::Ext::Button * buttonSave;
        Wt::Ext::Dialog * indirectoryChooser;
        Wt::Ext::Dialog * outdirectoryChooser;
        Wt::Ext::Button * selectInDirectory;
        Wt::Ext::Button * selectOutDirectory;
        Wt::Ext::Dialog * d;
        SqlTable * tab;
        Wt::WText * msg;
        int _user_id;
//        DirectoryFileFilter filter;
        FileTreeTable * intree;
        FileTreeTable * outtree;
        std::map<std::string, std::string> sqldata;
        std::map<std::string, Wt::Ext::LineEdit*> elements;

        map<std::string, int> name2id;
        map<int, std::string> id2name;
        std::map<int, int> profileid2profileidx;
        Wt::Ext::ComboBox * profiles;
        Wt::Ext::Button * encode;

        void openInDirectoryChooser() {
          indirectoryChooser->show();
        }

        void openOutDirectoryChooser() {
          outdirectoryChooser->show();
        }

        void enableEditButton() {
//          logdebug("Tab" << tab->selectedRows()[0]);
          /*
          int d = atoi(boost::any_cast<string > (tab->model()->data(tab->selectedRows()[0], 0)).c_str());
          SqlUtil::sql2map("watch_folder", d, sqldata);
          std::map<std::string, Wt::Ext::LineEdit*>::iterator it = elements.begin();
          for (; it != elements.end(); it++) {
            if((*it).first == "profile")
              ((Wt::Ext::ComboBox*)(*it).second)->setCurrentIndex(profileid2profileidx[atoi(sqldata[(*it).first].c_str())]);
            else
              (*it).second->setText(sqldata[(*it).first]);
          }*/
          buttonEdit->setEnabled(true);
        }

        void editWatchFolder() {
          d = new Wt::Ext::Dialog("Watchfolder");
          d->resize(480, 200);
          WatchfolderForm * pf = new WatchfolderForm(d->contents());
          int id = atoi(boost::any_cast<string > (tab->model()->data(tab->selectedRows()[0], 0)).c_str());
          pf->setWatchfolder(id);
//          pf->saved.connect(SLOT(d, Wt::Ext::Dialog::accept));
          pf->saved.connect(SLOT(this, WatchFolder::folderSaved));
          pf->canceled.connect(SLOT(d, Wt::Ext::Dialog::accept));
          d->show();
        }
        void folderSaved() {
          tab->reload("SELECT id,infolder, outfolder, profile from watch_folder");
          delete d;
        }
        void newWatchFolder() {
          std::map<std::string, Wt::Ext::LineEdit*>::iterator it = elements.begin();
          for (; it != elements.end(); it++) {
            (*it).second->setText("");
            //            (*it).second->setEnabled(true);
          }
          buttonSave->setEnabled(true);
          selectInDirectory->setEnabled(true);
          selectOutDirectory->setEnabled(true);
          elements["profile"]->setEnabled(true);
        }

        void selectInFolder() {
          logdebug("Folder Selected");

          Wt::WTree::WTreeNodeSet set = intree->tree()->selectedNodes();
          Wt::WTree::WTreeNodeSet::iterator it = set.begin();
          if (it != set.end()) {
            logdebug("Folder in set");
            FileTreeTableNode * node = (FileTreeTableNode*) * it;
            elements["infolder"]->setText(node->path_.string());
            logdebug(node->path_);
          }
        }

        void selectOutFolder() {
          logdebug("Folder Selected");

          Wt::WTree::WTreeNodeSet set = outtree->tree()->selectedNodes();
          Wt::WTree::WTreeNodeSet::iterator it = set.begin();
          if (it != set.end()) {
            logdebug("Folder in set");
            FileTreeTableNode * node = (FileTreeTableNode*) * it;
            elements["outfolder"]->setText(node->path_.string());
            logdebug(node->path_);
          }
        }

        void saveMap() {
          std::map<std::string, std::string> data;
          std::map<std::string, Wt::Ext::LineEdit*>::iterator it = elements.begin();
          data["id"] = elements["id"]->text().narrow();
          data["infolder"] = elements["infolder"]->text().narrow();
          data["outfolder"] = elements["outfolder"]->text().narrow();
          data["profile"] = Decimal(name2id[elements["profile"]->text().narrow()]).toString();
          /*
                    for (; it != elements.end(); it++) {
                      data[(*it).first] = (*it).second->text().narrow();
                    }
           */
          SqlUtil::map2sql("watch_folder", data);
          msg->setText("Data Saved");
        }

        Wt::Ext::LineEdit * buildElement(std::string name, std::string label, Wt::WTable * table, int row) {
          Wt::WLabel * elementLabel = new Wt::WLabel(label, table->elementAt(row, 0));
          table->elementAt(row, 0)->resize(Wt::WLength(14, Wt::WLength::FontEx), Wt::WLength());
          Wt::Ext::LineEdit * element = new Wt::Ext::LineEdit(table->elementAt(row, 1));
          element->setText(config::Config::getProperty((char*) name.c_str()));
          element->resize(Wt::WLength(50, Wt::WLength::FontEx), Wt::WLength());
          elementLabel->setBuddy(element);
          element->setText(sqldata[name]);
          elements[name] = element;
          return element;
        }
      };
    }
  }
}
