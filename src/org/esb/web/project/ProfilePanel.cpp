#include "ProfilePanel.h"

#include "Wt/WFitLayout"
#include "Wt/WStandardItemModel"
#include "Wt/Ext/TableView"
#include "Wt/Ext/ToolBar"

#include "ProfileChooser.h"
#include "ProfileCreator.h"
#include "org/esb/util/StringUtil.h"
#include "org/esb/config/config.h"
namespace org{
  namespace esb{
    namespace web{
      class ProfileTableModel:public Wt::WStandardItemModel{
      public:
        ProfileTableModel(){
          insertColumns(0, 2);
          setHeaderData(0, std::string("Id"));
          setHeaderData(1, std::string("Name"));

        }

        void setProfileData(std::vector<db::Profile> profiles){
          std::vector<db::Profile>::iterator it=profiles.begin();
          /*
          if(rowCount()>profiles.size()){
            removeRow(profiles.size()-1);
          }*/
          for(int a = 0;it!=profiles.end();it++, a++){
            if (rowCount() <= a)
              insertRow(rowCount());
            setData(a, 0, org::esb::util::StringUtil::toString((*it).id));
            setData(a, 1, (std::string)(*it).name);
          }
        }
      };
      class ProfileTable:public Wt::Ext::TableView{
      public:
        ProfileTable(){
          setModel(new ProfileTableModel());
          setColumnWidth(0,50);
          setColumnWidth(1,300);
          
         
        }
        void setProfiles(std::vector<db::Profile> profiles){
          ProfileTableModel*oldptr=static_cast<ProfileTableModel*>(model());
          setModel(new ProfileTableModel());
          delete oldptr;
          static_cast<ProfileTableModel*>(model())->setProfileData(profiles);
        }
      };

      ProfilePanel::ProfilePanel():Wt::Ext::Panel(){
        setTitle("Output Profiles");
        setLayout(new Wt::WFitLayout());
        setToolTip("Click on \"Add Output Profile\" to select an Output Profile!");
        layout()->setContentsMargins(0, 0, 0, 0);
        setTopToolBar(new Wt::Ext::ToolBar());
        _profile_table=Ptr<ProfileTable>(new ProfileTable());

        _profile_table->setAlternatingRowColors(true);
        _profile_table->setHighlightMouseOver(true);
        _profile_table->setSelectionBehavior(Wt::SelectRows);
        _profile_table->setSelectionMode(Wt::SingleSelection);
        _profile_table->itemSelectionChanged.connect(SLOT(this, ProfilePanel::enableButtons));
        layout()->addWidget(_profile_table.get());

        Wt::Ext::Button * addProfileButton = topToolBar()->addButton("Add Output Profile");
        addProfileButton ->clicked.connect(SLOT(this, ProfilePanel::addProfileButtonClicked));
        topToolBar()->addSeparator();
        editProfileButton = topToolBar()->addButton("Edit Output Profile");
        editProfileButton ->clicked.connect(SLOT(this, ProfilePanel::editSelectedProfile));
        editProfileButton->setEnabled(false);
        topToolBar()->addSeparator();
        removeProfileButton = topToolBar()->addButton("Remove Output Profile");
        removeProfileButton ->clicked.connect(SLOT(this, ProfilePanel::removeSelectedProfile));
        removeProfileButton->setEnabled(false);
      }

      void ProfilePanel::setProject(Ptr<db::Project> p){
        _project=p;
        _profile_table->setProfiles(_project->profiles().get().all());
      }

      void ProfilePanel::addProfileButtonClicked(){
        _profile_chooser=Ptr<ProfileChooser>(new ProfileChooser());
        _profile_chooser->selected.connect(SLOT(this,ProfilePanel::profileChooserSelected));
        _profile_chooser->show();
      }

      void ProfilePanel::removeSelectedProfile(){
        int pid=atoi(boost::any_cast<string > (_profile_table->model()->data(_profile_table->selectedRows()[0], 0)).c_str());
        db::HiveDb db("mysql",org::esb::config::Config::getProperty("db.url"));
        db::Profile profile=litesql::select<db::Profile>(db, db::Profile::Id==pid).one();
        _project->profiles().unlink(profile);
        _profile_table->setProfiles(_project->profiles().get().all());
        removeProfileButton->setEnabled(false);
        editProfileButton->setEnabled(false);
      }

      void ProfilePanel::editSelectedProfile(){
        int pid=atoi(boost::any_cast<string > (_profile_table->model()->data(_profile_table->selectedRows()[0], 0)).c_str());

        _profile=Ptr<db::Profile>(new db::Profile(litesql::select<db::Profile>(_project->getDatabase(), db::Profile::Id==pid).one()));
        _profile_editor=Ptr<ProfileCreator>(new ProfileCreator(*_profile.get()));
        _profile_editor->saved.connect(SLOT(this, ProfilePanel::profileSaved));
        _profile_editor->show();
        
      }
      void ProfilePanel::profileSaved(){
        _profile->update();
      }

      void ProfilePanel::enableButtons(){
        if(_profile_table->selectedRows().size()>0){
        removeProfileButton->setEnabled(true);
        editProfileButton->setEnabled(true);
        }
      }

      void ProfilePanel::profileChooserSelected(){
        int c=_profile_chooser->getSelectedProfileId();
        
        db::HiveDb db("mysql",org::esb::config::Config::getProperty("db.url"));
        db::Profile profile=litesql::select<db::Profile>(db, db::Profile::Id==c).one();
        _project->profiles().link(profile);
        _profile_table->setProfiles(_project->profiles().get().all());
        removeProfileButton->setEnabled(false);
        editProfileButton->setEnabled(false);
        LOGDEBUG("ProfileId:"<<c);
      }
    }
  }
}
