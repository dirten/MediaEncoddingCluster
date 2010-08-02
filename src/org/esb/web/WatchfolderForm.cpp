
#include "WatchfolderForm.h"
#include "org/esb/util/StringUtil.h"
#include "SqlUtil.h"
#include "org/esb/sql/Connection.h"
#include "org/esb/sql/Statement.h"
#include "org/esb/sql/ResultSet.h"

#include "org/esb/config/config.h"
#include <Wt/WTable>
#include <Wt/WFitLayout>
#include <Wt/WBreak>
#include <Wt/Ext/Button>
#include <Wt/Ext/Dialog>

#include "DirectoryChooser.h"
namespace org {
  namespace esb {
    namespace web {

      WatchfolderForm::WatchfolderForm(db::Watchfolder & folder) : Wt::Ext::Dialog("Watchfolder"),_folder(folder) {

        int i = 0;
        Wt::WFitLayout *fit=new Wt::WFitLayout();
        setLayout(fit);

        Wt::WContainerWidget *cont=new Wt::WContainerWidget();
        fit->addWidget(cont);


        Wt::WGridLayout * l = new Wt::WGridLayout();
        //        l->setVerticalSpacing(3);
        //        l->setHorizontalSpacing(0);
        //        l->setMinimumSize(Wt::WLength::Auto, 30);
        resize(480, 230);
        cont->setLayout(l);
        _le.addElement("id", "Watchfolder Id", folder.id, l)->setEnabled(false);

        _le.addElement("infolder", "Input Folder", folder.infolder , l)->setEnabled(false);
        Wt::Ext::Button * selectInDirectory = new Wt::Ext::Button("Select Directory");
        //        selectInDirectory->resize(Wt::WLength::Auto, 30);
        l->addWidget(selectInDirectory, 1, 2);
        selectInDirectory->clicked.connect(SLOT(this, WatchfolderForm::openInfolder));

        _le.addElement("outfolder", "Output Folder", folder.outfolder, l)->setEnabled(false);
        Wt::Ext::Button * selectOutDirectory = new Wt::Ext::Button("Select Directory");
        l->addWidget(selectOutDirectory, 2, 2);
        selectOutDirectory->clicked.connect(SLOT(this, WatchfolderForm::openOutfolder));

        Wt::Ext::ComboBox * profiles = _cb.addElement("profile", "Encoding Profile", "", l);
        profiles->setTextSize(50);
        {
          int pid=-1;
          if(folder.isInDatabase()&&folder.profile().get().count()>0)
            pid=folder.profile().get().one().id;
        db::HiveDb db("mysql",org::esb::config::Config::getProperty("db.url"));
        vector<db::Profile> plist=litesql::select<db::Profile>(db).all();
        vector<db::Profile>::iterator it=plist.begin();
        for(int a = 0;it!=plist.end();it++, a++){
          profiles->addItem((std::string)(*it).name);
          name2id[(std::string)(*it).name] = (*it).id;
          id2name[(*it).id] = (*it).name;
          profileid2profileidx[(*it).id] = a;
          if(pid==(*it).id)
            profiles->setCurrentIndex(a);
        }
        }
//        _cb.getElement("profile")->setCurrentIndex(profileid2profileidx[(int)folder.profile().get().one().id]);
        /*
        while (rs.next()) {
        profiles->addItem(rs.getString("profile_name"));
        name2id[rs.getString("profile_name")] = rs.getInt("id");
        id2name[rs.getInt("id")] = rs.getString("profile_name");
        profileid2profileidx[rs.getInt("id")] = a++;
        }*/
        _le.addElement("extension_filter", "Extension Filter", folder.extensionfilter, l);

        i++;
        //        new Wt::WBreak(parent);
        //        Wt::WTable * b = new Wt::WTable(parent);
        addButton(new Wt::Ext::Button("Save"));
        buttons().back()->clicked.connect(SLOT(this, WatchfolderForm::save));

        addButton(new Wt::Ext::Button("Cancel"));
        buttons().back()->clicked.connect(SLOT(this, WatchfolderForm::cancel));
        std::map<std::string, Wt::Ext::LineEdit*> lel = _le.getElements();
        std::map<std::string, Wt::Ext::LineEdit*>::iterator it = lel.begin();
        for (; it != lel.end(); it++) {
          (*it).second->keyPressed.connect(SLOT(this,WatchfolderForm::changed));
          (*it).second->keyWentUp.connect(SLOT(this,WatchfolderForm::changed));
          
        }
        std::map<std::string, Wt::Ext::ComboBox*> lec = _cb.getElements();
        std::map<std::string, Wt::Ext::ComboBox*>::iterator itc = lec.begin();
        for (; itc != lec.end(); itc++) {
          (*itc).second->keyPressed.connect(SLOT(this,WatchfolderForm::changed));
          (*itc).second->activated.connect(SLOT(this,WatchfolderForm::changed));

        }

        /*
        Wt::Ext::Button *cancel = new Wt::Ext::Button("Cancel");
        Wt::Ext::Button *save = new Wt::Ext::Button("Save");
        l->addWidget(cancel, 5, 0);
        l->addWidget(save, 5, 1);

        cancel->clicked.connect(SLOT(this, WatchfolderForm::cancel));
        save->clicked.connect(SLOT(this, WatchfolderForm::save));
        */
      }

      void WatchfolderForm::save() {
        changed();
        saved.emit();
        this->done(Accepted);
      }

      void WatchfolderForm::changed() {

        _folder.infolder=_le.getElement("infolder")->text().narrow();
        _folder.outfolder=_le.getElement("outfolder")->text().narrow();
        _folder.extensionfilter=_le.getElement("extension_filter")->text().narrow();
        _folder.profile().del();
        _folder.update();
        db::HiveDb db("mysql",org::esb::config::Config::getProperty("db.url"));
        int pid=name2id[_cb.getElement("profile")->text().narrow()];
        try{
          db::Profile pr=litesql::select<db::Profile>(db, db::Profile::Id==pid).one();
          _folder.profile().link(pr);
        }catch(litesql::NotFound & ex){
          LOGINFO("no profile set for Watchfolder:"<<ex.what());
        }
        _folder.update();
        LOGDEBUG(_folder)
      }

      void WatchfolderForm::openInfolder() {
        dc = new DirectoryChooser("Select Infolder");
        dc->selected.connect(SLOT(this, WatchfolderForm::selectedInfolder));
        dc->canceled.connect(SLOT(dc, Wt::Ext::Dialog::accept));
        dc->show();
      }

      void WatchfolderForm::selectedInfolder(std::string path) {
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
        _le.getElement("outfolder")->setText(path);
        delete dc;
      }

      void WatchfolderForm::cancel() {
        canceled.emit();
        this->done(Rejected);
      }

      void WatchfolderForm::setWatchfolder(db::Watchfolder & folder) {
        _le.getElement("id")->setText(org::esb::util::StringUtil::toString((int)folder.id));
        _le.getElement("infolder")->setText((std::string)folder.infolder);
        _le.getElement("outfolder")->setText((std::string)folder.outfolder);
        _le.getElement("extension_filter")->setText((std::string)folder.extensionfilter);
        _cb.getElement("profile")->setCurrentIndex(profileid2profileidx[(int)folder.profile().get().one().id]);
      }
      void WatchfolderForm::setWatchfolder(int id) {
        /*
        std::map<std::string, std::string> sqldata;
        SqlUtil::sql2map("watch_folder", id, sqldata);
        _le.getElement("id")->setText(sqldata["id"]);
        _le.getElement("infolder")->setText(sqldata["infolder"]);
        _le.getElement("outfolder")->setText(sqldata["outfolder"]);
        _le.getElement("extension_filter")->setText(sqldata["extension_filter"]);
        _cb.getElement("profile")->setCurrentIndex(profileid2profileidx[atoi(sqldata["profile"].c_str())]);*/
      }
    }
  }
}

