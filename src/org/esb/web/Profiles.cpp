#include "org/esb/db/hivedb.hpp"
#include "org/esb/config/config.h"
#include <Wt/WWidget>
#include <Wt/WLength>
#include <Wt/WContainerWidget>
#include <Wt/WFitLayout>
#include <Wt/Ext/Button>
#include <Wt/Ext/ToolBar>
#include <Wt/Ext/Dialog>
#include "SqlTable.h"
#include "ProfilesForm.h"
#include "ProfileCreator.h"
#include "DbTable.h"
//#include "org/esb/lang/Object.h"
namespace org {
  namespace esb {
    namespace web {

      class Profiles : public Wt::Ext::Container {
      public:

        Profiles(Wt::WContainerWidget * parent = 0) : Wt::Ext::Container() {
          Wt::WFitLayout * l = new Wt::WFitLayout();
          setLayout(l);
          list<ColumnConfig> columnConfigs;
          columnConfigs.push_back(ColumnConfig(db::Profile::Id,"Id" ,20));
          columnConfigs.push_back(ColumnConfig(db::Profile::Name,"Name" ,200));
          columnConfigs.push_back(ColumnConfig(db::Profile::Format,"Format" ,200));
          //          columnConfigs.push_back(ColumnConfig(db::Profile::Containertype,"Type" ,200));

          profile_table=new DbTable(columnConfigs,litesql::Expr());

          //          t = new SqlTable(std::string("select id, profile_name Name, v_format Format from profiles"));
          //          t->setColumnWidth(0, 10);
          //          t->itemSelectionChanged.connect(SLOT(this, Profiles::enableButton));
          //          t->setTopToolBar(new Wt::Ext::ToolBar());

          profile_table->itemSelectionChanged.connect(SLOT(this, Profiles::enableButton));
          profile_table->doubleClicked.connect(SLOT(this, Profiles::editProfile));
          profile_table->setTopToolBar(new Wt::Ext::ToolBar());

          layout()->addWidget(profile_table);

          buttonEdit = profile_table->topToolBar()->addButton("Edit selected Profile");
          buttonEdit->setEnabled(false);
          buttonEdit->clicked.connect(SLOT(this, Profiles::editProfile));
          profile_table->topToolBar()->addSeparator();
          buttonDelete = profile_table->topToolBar()->addButton("Delete selected Profile");
          buttonDelete->setEnabled(false);
          buttonDelete->clicked.connect(SLOT(this, Profiles::deleteProfile));
        }

        void enableButton() {
          if (profile_table->selectedRows().size() > 0) {
            //            int d = atoi(boost::any_cast<string > (profile_table->model()->data(profile_table->selectedRows()[0], 0)).c_str());
            buttonEdit->setEnabled(true);
          }
        }

        void reloadProfiles() {
          profile_table->reload();
        }

        void deleteProfile() {
          /*confirm deleting*/
        }

        void editProfile() {
          //	  LOGDEBUG("here");
          int c = atoi(boost::any_cast<string > (profile_table->getModel()->data(profile_table->selectedRows()[0], 0)).c_str());

          _db=boost::shared_ptr<db::HiveDb>(new db::HiveDb("mysql",org::esb::config::Config::getProperty("db.url")));
          _db->verbose=true;
          profile=new db::Profile(litesql::select<db::Profile>(*_db.get(), db::Profile::Id==c).one());
          pc=boost::shared_ptr<ProfileCreator>(new ProfileCreator(*profile));
          //          pc->show();
          /*
          d = new Wt::Ext::Dialog("Profile");
          d->resize(500, 430);
          ProfilesForm * pf = new ProfilesForm(d->contents());
          */
          pc->saved.connect(SLOT(this, Profiles::profileSaved));
          pc->canceled.connect(SLOT(this, Profiles::profileCanceled));
          //          pc->canceled.connect(SLOT(pc.get(), Wt::Ext::Dialog::accept));
          //          pc->setProfile(c);
          pc->show();
          LOGDEBUG(*profile)
        }
        void createProfile() {
          //	  LOGDEBUG("here");
        

          _db=boost::shared_ptr<db::HiveDb>(new db::HiveDb("mysql",org::esb::config::Config::getProperty("db.url")));
          _db->verbose=true;
          profile=new db::Profile(*_db.get());
          pc=boost::shared_ptr<ProfileCreator>(new ProfileCreator(*profile));
          //          pc->show();
          /*
          d = new Wt::Ext::Dialog("Profile");
          d->resize(500, 430);
          ProfilesForm * pf = new ProfilesForm(d->contents());
          */
          pc->saved.connect(SLOT(this, Profiles::profileSaved));
          pc->canceled.connect(SLOT(this, Profiles::profileCanceled));
          //          pc->canceled.connect(SLOT(pc.get(), Wt::Ext::Dialog::accept));
          //          pc->setProfile(c);
          pc->show();
          LOGDEBUG(*profile)
        }
        void profileSaved(){
          reloadProfiles();
          profile->update();
        }
        void profileCanceled(){
          
        }
      private:
        Wt::Ext::Button * buttonEdit;
        Wt::Ext::Button * buttonNew;
        Wt::Ext::Button * buttonDelete;
        //        SqlTable * t;
        DbTable * profile_table;
        Wt::Ext::Dialog * d;
        db::Profile * profile;
        boost::shared_ptr<ProfileCreator> pc;
        boost::shared_ptr<db::HiveDb> _db;
        //        ProfilesEdit * edit;
      };
    }
  }
}

