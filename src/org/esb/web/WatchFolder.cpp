#include <Wt/Ext/ToolBar>
#include <Wt/Ext/Dialog>
#include <Wt/WTree>
#include "FileTreeTable.h"
#include "FileTreeTableNode.h"

#include "org/esb/config/config.h"
#include "org/esb/io/FileFilter.h"
#include "org/esb/io/File.h"

#include <boost/algorithm/string.hpp>

namespace org {
  namespace esb {
    namespace web {
      class DirectoryFileFilter : public org::esb::io::FileFilter {
      public:
        bool accept(org::esb::io::File file) {
          return file.isDirectory();
        }
      };

      class WatchFolder : public Wt::WContainerWidget {
      public:

        WatchFolder(int user_id) : Wt::WContainerWidget(NULL) {
          using namespace org::esb::config;
          _user_id = user_id;
          tab = new SqlTable(std::string("SELECT * from watch_folder"), this);
          tab->setTopToolBar(new Wt::Ext::ToolBar());
          tab->resize(Wt::WLength(), 300);
          buttonEdit = tab->topToolBar()->addButton("Edit selected Watch Folder");
          buttonNew = tab->topToolBar()->addButton("Create new Watch Folder");
          Wt::WGroupBox * group = new Wt::WGroupBox("WatchFolder", this);
          Wt::WTable *t = new Wt::WTable(group);

          buttonEdit->setEnabled(false);
          buttonNew->setEnabled(true);
          buttonEdit->clicked.connect(SLOT(this, WatchFolder::editWatchFolder));
          buttonNew->clicked.connect(SLOT(this, WatchFolder::newWatchFolder));

          int i = 0;
          buildElement("id", "Id", t, i++)->setEnabled(false);
          buildElement("folder", "Watch Folder", t, i++)->setEnabled(false);
          selectDirectory = new Wt::Ext::Button("Select Directory", t->elementAt(i - 1, 2));
          selectDirectory->setEnabled(false);
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

          directoryChooser = new Wt::Ext::Dialog("Choose Directory");
//          DirectoryFileFilter * filter=new DirectoryFileFilter();
          tree = new FileTreeTable(Config::getProperty("hive.scandir"),filter, directoryChooser->contents());
          tree->resize(500,300);

          tree->tree()->itemSelectionChanged.connect(SLOT(this,WatchFolder::selectFolder));
          
          Wt::Ext::Button *select = new Wt::Ext::Button("Select", directoryChooser->contents());
          select->clicked.connect(SLOT(directoryChooser, Wt::Ext::Dialog::accept));
          directoryChooser->resize(600, 400);
          selectDirectory->clicked.connect(SLOT(this, WatchFolder::openDirectoryChooser));
          tab->itemSelectionChanged.connect(SLOT(this, WatchFolder::enableEditButton));
        }
        ~WatchFolder(){
          
        }

      private:
        Wt::Ext::Button * buttonEdit;
        Wt::Ext::Button * buttonNew;
        Wt::Ext::Button * buttonSave;
        Wt::Ext::Dialog * directoryChooser;
        Wt::Ext::Button * selectDirectory;
        SqlTable * tab;
        Wt::WText * msg;
        int _user_id;
        DirectoryFileFilter filter;
        FileTreeTable * tree;
        std::map<std::string, std::string> sqldata;
        std::map<std::string, Wt::Ext::LineEdit*> elements;
        
        map<std::string, int> name2id;
        map<int, std::string> id2name;
        std::map<int, int> profileid2profileidx;
        Wt::Ext::ComboBox * profiles;
        Wt::Ext::Button * encode;

        void openDirectoryChooser() {
          directoryChooser->show();
        }

        void enableEditButton() {
          logdebug("Tab" << tab->selectedRows()[0]);
          int d = atoi(boost::any_cast<string > (tab->model()->data(tab->selectedRows()[0], 0)).c_str());
          SqlUtil::sql2map("watch_folder", d, sqldata);
          std::map<std::string, Wt::Ext::LineEdit*>::iterator it = elements.begin();
          for (; it != elements.end(); it++) {
            if((*it).first == "profile")
              ((Wt::Ext::ComboBox*)(*it).second)->setCurrentIndex(profileid2profileidx[atoi(sqldata[(*it).first].c_str())]);
            else
              (*it).second->setText(sqldata[(*it).first]);
          }
          buttonEdit->setEnabled(true);
        }

        void editWatchFolder() {
          std::map<std::string, Wt::Ext::LineEdit*>::iterator it = elements.begin();
          for (; it != elements.end(); it++) {
//            (*it).second->setEnabled(true);
          }
          buttonSave->setEnabled(true);
          selectDirectory->setEnabled(true);
          elements["profile"]->setEnabled(true);
        }

        void newWatchFolder() {
          std::map<std::string, Wt::Ext::LineEdit*>::iterator it = elements.begin();
          for (; it != elements.end(); it++) {
            (*it).second->setText("");
//            (*it).second->setEnabled(true);
          }
          buttonSave->setEnabled(true);
          selectDirectory->setEnabled(true);
		  elements["profile"]->setEnabled(true);
        }

        void selectFolder(){
          logdebug("Folder Selected");

          Wt::WTree::WTreeNodeSet set=tree->tree()->selectedNodes();
          Wt::WTree::WTreeNodeSet::iterator it=set.begin();
          if(it!=set.end()){
          logdebug("Folder in set");
            FileTreeTableNode * node=(FileTreeTableNode*)*it;
            elements["folder"]->setText(node->path_.string());
            logdebug(node->path_);
          }
        }
        void saveMap() {
          std::map<std::string, std::string> data;
          std::map<std::string, Wt::Ext::LineEdit*>::iterator it = elements.begin();
          data["id"]=elements["id"]->text().narrow();
          data["folder"]=elements["folder"]->text().narrow();
          data["profile"]=Decimal(name2id[elements["profile"]->text().narrow()]).toString() ;
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
          element->resize(Wt::WLength(30, Wt::WLength::FontEx), Wt::WLength());
          elementLabel->setBuddy(element);
          element->setText(sqldata[name]);
          elements[name] = element;
          return element;
        }
      };
    }
  }
}
