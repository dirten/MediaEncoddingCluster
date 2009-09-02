#include "WatchfolderForm.h"

#include "SqlUtil.h"
#include "org/esb/sql/Connection.h"
#include "org/esb/sql/Statement.h"
#include "org/esb/sql/ResultSet.h"

#include "org/esb/config/config.h"
#include <Wt/WTable>
#include <Wt/WBreak>
#include <Wt/Ext/Button>
#include <Wt/Ext/Dialog>

#include "DirectoryChooser.h"
namespace org {
  namespace esb {
    namespace web {

      WatchfolderForm::WatchfolderForm(Wt::WContainerWidget * parent) : Wt::WContainerWidget(parent) {
        Wt::WTable * t = new Wt::WTable(parent);
        int i = 0;
        _le.getElement("id", "Watchfolder Id", "", t->elementAt(i++, 0))->setEnabled(false);

        _le.getElement("infolder", "Input Folder", "", t->elementAt(i++, 0))->setEnabled(false);
        Wt::Ext::Button * selectInDirectory = new Wt::Ext::Button("Select Directory", t->elementAt(i - 1, 2));
        selectInDirectory->clicked.connect(SLOT(this, WatchfolderForm::openInfolder));

        _le.getElement("outfolder", "Output Folder", "", t->elementAt(i++, 0))->setEnabled(false);
        Wt::Ext::Button * selectOutDirectory = new Wt::Ext::Button("Select Directory", t->elementAt(i - 1, 2));
        selectOutDirectory->clicked.connect(SLOT(this, WatchfolderForm::openOutfolder));

        Wt::Ext::ComboBox * profiles = _cb.getElement("profile", "Encoding Profile", "", t->elementAt(i++, 0));
        org::esb::sql::Connection con(std::string(org::esb::config::Config::getProperty("db.connection")));

        org::esb::sql::Statement stmt = con.createStatement("select * from profiles");
        org::esb::sql::ResultSet rs = stmt.executeQuery();
        int a = 0;
        while (rs.next()) {
          profiles->addItem(rs.getString("profile_name"));
          name2id[rs.getString("profile_name")] = rs.getInt("id");
          id2name[rs.getInt("id")] = rs.getString("profile_name");
          profileid2profileidx[rs.getInt("id")] = a++;
        }
        i++;
        new Wt::WBreak(parent);
        Wt::WTable * b = new Wt::WTable(parent);
        Wt::Ext::Button *cancel = new Wt::Ext::Button("Cancel", b->elementAt(0, 0));
        Wt::Ext::Button *save = new Wt::Ext::Button("Save", b->elementAt(0, 1));
        cancel->clicked.connect(SLOT(this, WatchfolderForm::cancel));
        save->clicked.connect(SLOT(this, WatchfolderForm::save));

      }

      void WatchfolderForm::save() {
        std::map<std::string, std::string> data;
        data["id"] = _le.getElement("id")->text().narrow();
        data["infolder"] = _le.getElement("infolder")->text().narrow();
        data["outfolder"] = _le.getElement("outfolder")->text().narrow();
        data["profile"] = Decimal(name2id[_cb.getElement("profile")->text().narrow()]).toString();
        SqlUtil::map2sql("watch_folder", data);
        saved.emit();
      }

      void WatchfolderForm::openInfolder() {
        dc = new DirectoryChooser("Select Infolder");
        dc->selected.connect(SLOT(this, WatchfolderForm::selectedInfolder ));
        dc->canceled.connect(SLOT(dc, Wt::Ext::Dialog::accept));
        dc->show();
      }

      void WatchfolderForm::selectedInfolder(std::string path) {
        logdebug("selected Path:"<<path);
        _le.getElement("infolder")->setText(path);
        delete dc;
      }

      void WatchfolderForm::openOutfolder() {
        dc = new DirectoryChooser("Select Outfolder");
        dc->selected.connect(SLOT(this, WatchfolderForm::selectedOutfolder ));
        dc->canceled.connect(SLOT(dc, Wt::Ext::Dialog::accept));
        dc->show();
      }

      void WatchfolderForm::selectedOutfolder(std::string path) {
        logdebug("selected Path:"<<path);
        _le.getElement("outfolder")->setText(path);
        delete dc;
      }


      void WatchfolderForm::cancel() {
        canceled.emit();
      }

      void WatchfolderForm::setWatchfolder(int id) {
        std::map<std::string, std::string> sqldata;
        SqlUtil::sql2map("watch_folder", id, sqldata);
        _le.getElement("id")->setText(sqldata["id"]);
        _le.getElement("infolder")->setText(sqldata["infolder"]);
        _le.getElement("outfolder")->setText(sqldata["outfolder"]);
        _cb.getElement("profile")->setCurrentIndex(profileid2profileidx[atoi(sqldata["profile"].c_str())]);
      }
    }
  }
}

