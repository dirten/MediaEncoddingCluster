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
//        Wt::WTable * t = new Wt::WTable(parent);
        int i = 0;
        Wt::WGridLayout * l = new Wt::WGridLayout();
//        l->setVerticalSpacing(3);
//        l->setHorizontalSpacing(0);
//        l->setMinimumSize(Wt::WLength::Auto, 30);
        setLayout(l);
        _le.addElement("id", "Watchfolder Id", "", l)->setEnabled(false);

        _le.addElement("infolder", "Input Folder", "", l)->setEnabled(false);
        Wt::Ext::Button * selectInDirectory = new Wt::Ext::Button("Select Directory");
//        selectInDirectory->resize(Wt::WLength::Auto, 30);
        l->addWidget(selectInDirectory,1,2);
        selectInDirectory->clicked.connect(SLOT(this, WatchfolderForm::openInfolder));

        _le.addElement("outfolder", "Output Folder", "", l)->setEnabled(false);
        Wt::Ext::Button * selectOutDirectory = new Wt::Ext::Button("Select Directory");
        l->addWidget(selectOutDirectory,2,2);
        selectOutDirectory->clicked.connect(SLOT(this, WatchfolderForm::openOutfolder));

        Wt::Ext::ComboBox * profiles = _cb.addElement("profile", "Encoding Profile", "", l);
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
//        new Wt::WBreak(parent);
//        Wt::WTable * b = new Wt::WTable(parent);
        Wt::Ext::Button *cancel = new Wt::Ext::Button("Cancel");
        Wt::Ext::Button *save = new Wt::Ext::Button("Save");
        l->addWidget(cancel,4,0);
        l->addWidget(save,4,1);

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
        dc->selected.connect(SLOT(this, WatchfolderForm::selectedInfolder));
        dc->canceled.connect(SLOT(dc, Wt::Ext::Dialog::accept));
        dc->show();
      }

      void WatchfolderForm::selectedInfolder(std::string path) {
        logdebug("selected Path:" << path);
        _le.getElement("infolder")->setText(path);
        delete dc;
      }

      void WatchfolderForm::openOutfolder() {
        dc = new DirectoryChooser("Select Outfolder");
        dc->selected.connect(SLOT(this, WatchfolderForm::selectedOutfolder));
        dc->canceled.connect(SLOT(dc, Wt::Ext::Dialog::accept));
        dc->show();
      }

      void WatchfolderForm::selectedOutfolder(std::string path) {
        logdebug("selected Path:" << path);
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

