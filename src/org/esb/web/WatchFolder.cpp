#include <Wt/Ext/ToolBar>
#include <Wt/Ext/Dialog>

#include "FileTreeTable.h"
#include "FileTreeTable.h"
#include "org/esb/config/config.h"
namespace org {
  namespace esb {
    namespace web {

      class WatchFolder : public Wt::WContainerWidget {
      public:
      
        WatchFolder(int user_id) : Wt::WContainerWidget(NULL) {
          using namespace org::esb::config;
          _user_id = user_id;
          tab = new SqlTable(std::string("SELECT * from watch_folder"), this);
          tab->setTopToolBar(new Wt::Ext::ToolBar());
          tab->resize(Wt::WLength(), 300);
          tab->itemSelectionChanged.connect(SLOT(this, WatchFolder::enableEditButton));
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
          buildElement("profile", "Profile", t, i++)->setEnabled(false);
          msg = new Wt::WText(t->elementAt(i, 0));
          buttonSave = new Wt::Ext::Button("Save", t->elementAt(i, 1));
          buttonSave->clicked.connect(SLOT(this, WatchFolder::saveMap));
          buttonSave->setEnabled(false);
          directoryChooser=new Wt::Ext::Dialog("Choose Directory");
          FileTreeTable * tree=new FileTreeTable(Config::getProperty("hive.basescandir"), directoryChooser->contents());
          Wt::Ext::Button *select=new Wt::Ext::Button("Select", directoryChooser->contents());
          select->clicked.connect(SLOT(directoryChooser, Wt::Ext::Dialog::accept));
          directoryChooser->resize(600,400);
        }
      private:
        Wt::Ext::Button * buttonEdit;
        Wt::Ext::Button * buttonNew;
        Wt::Ext::Button * buttonSave;
        Wt::Ext::Dialog * directoryChooser;
        SqlTable * tab;
        Wt::WText * msg;
        int _user_id;
        std::map<std::string, std::string> sqldata;
        std::map<std::string, Wt::Ext::LineEdit*> elements;

        void enableEditButton() {
          directoryChooser->show();
          logdebug("Tab"<<tab->selectedRows()[0]);
          int d=atoi(boost::any_cast<string>(tab->model()->data(tab->selectedRows()[0],0)).c_str());
          SqlUtil::sql2map("watch_folder", d, sqldata);
          std::map<std::string, Wt::Ext::LineEdit*>::iterator it = elements.begin();
          for (; it != elements.end(); it++) {
            (*it).second->setText(sqldata[(*it).first]);
          }
          buttonEdit->setEnabled(true);
        }

        void editWatchFolder() {
          std::map<std::string, Wt::Ext::LineEdit*>::iterator it = elements.begin();
          for (; it != elements.end(); it++) {
            (*it).second->setEnabled(true);
          }
          buttonSave->setEnabled(true);
        }

        void newWatchFolder() {
          std::map<std::string, Wt::Ext::LineEdit*>::iterator it = elements.begin();
          for (; it != elements.end(); it++) {
            (*it).second->setText("");
            (*it).second->setEnabled(true);
          }
          buttonSave->setEnabled(true);
        }

        void saveMap() {
          std::map<std::string, std::string> data;
          std::map<std::string, Wt::Ext::LineEdit*>::iterator it = elements.begin();
          for (; it != elements.end(); it++) {
            data[(*it).first] = (*it).second->text().narrow();
          }
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
