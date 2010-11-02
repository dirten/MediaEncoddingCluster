#include "ProfileChooser.h"

#include "Wt/WFitLayout"
#include "Wt/Ext/Button"
#include "Wt/WStandardItemModel"
#include "Wt/Ext/TableView"
#include "org/esb/util/StringUtil.h"
#include "org/esb/config/config.h"
#include "org/esb/hive/DatabaseService.h"

namespace org{
  namespace esb{
    namespace web{

      class ProfileTableModel:public Wt::WStandardItemModel{
      public:
        ProfileTableModel(){
          insertColumns(0, 3);
          setHeaderData(0, std::string("Id"));
          setHeaderData(1, std::string("Name"));

        }

        void setProfileData(std::vector<db::Profile> profiles){
          std::vector<db::Profile>::iterator it=profiles.begin();
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
          static_cast<ProfileTableModel*>(model())->setProfileData(profiles);
        }
      };

      ProfileChooser::ProfileChooser():Wt::Ext::Dialog("Profile Chooser"){
        setLayout(new Wt::WFitLayout());
        resize(400,300);
        _profile_table=Ptr<ProfileTable>(new ProfileTable());
        _presetList=new PresetList();
        _presetList->presetSelected.connect(SLOT(this, ProfileChooser::presetSelected));
        layout()->addWidget(_presetList.get());

        /*
        db::HiveDb db=org::esb::hive::DatabaseService::getDatabase();
        vector<db::Profile> profiles=litesql::select<db::Profile>(db).all();
        _profile_table->setProfiles(profiles);
        _profile_table->setAlternatingRowColors(true);
//        _profile_table->resizeColumnsToContents(true);
        _profile_table->setHighlightMouseOver(true);
        _profile_table->setSelectionBehavior(Wt::SelectRows);
        _profile_table->setSelectionMode(Wt::SingleSelection);
         */
        addButton(new Wt::Ext::Button("Select Profile"));
        buttons().back()->clicked().connect(SLOT(this, ProfileChooser::choose));

        addButton(new Wt::Ext::Button("Cancel"));
        buttons().back()->clicked().connect(SLOT(this, ProfileChooser::cancel));

      }

      ProfileChooser::~ProfileChooser(){

      }

      int ProfileChooser::getSelectedProfileId(){
        return selected_profile_id;
      }
      
      std::string ProfileChooser::getSelectedProfile(){
        return _current_selected_preset;
      }

      void ProfileChooser::presetSelected(std::string filename){
        _current_selected_preset=filename;
      }

      void ProfileChooser::choose(){
        //selected_profile_id = atoi(boost::any_cast<string > (_profile_table->model()->data(_profile_table->selectedRows()[0], 0)).c_str());
        selected.emit();
        this->done(Accepted);
      }

      void ProfileChooser::cancel(){
        canceled.emit();
        this->done(Rejected);
      }
    }
  }
}
