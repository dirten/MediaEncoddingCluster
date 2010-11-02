#include "ProfilePanel.h"

#include "Wt/WFitLayout"
#include "Wt/WStandardItemModel"
#include "Wt/Ext/TableView"
#include "Wt/Ext/ToolBar"

#include "ProfileChooser.h"
#include "ProfileCreator.h"
#include "org/esb/util/StringUtil.h"
#include "org/esb/config/config.h"
#include "org/esb/hive/DatabaseService.h"
#include "org/esb/web/ProfileChooser.h"
#include "org/esb/hive/PresetReader.h"
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

        void setPresetData(std::vector<db::Preset> presets){
          std::vector<db::Preset>::iterator it=presets.begin();
          
          if(rowCount()>presets.size()){
            if(presets.size()>0)
              removeRow(presets.size()-1);
            else{
              removeRows(0,rowCount());
            }
          }
          for(int a = 0;it!=presets.end();it++, a++){
            if (rowCount() <= a)
              insertRow(rowCount());
            setData(a, 0, (*it).filename.value());
            setData(a, 1, (*it).name.value());
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
        void refresh(){
          Wt::Ext::TableView::refresh();
        }

        void setPresets(std::vector<db::Preset> presets){
          static_cast<ProfileTableModel*>(model())->setPresetData(presets);
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
        _profile_table->itemSelectionChanged().connect(SLOT(this, ProfilePanel::enableButtons));
        layout()->addWidget(_profile_table.get());

        Wt::Ext::Button * addProfileButton = topToolBar()->addButton("Add Output Profile");
        addProfileButton->setIcon("icons/add-icon.png");
        addProfileButton ->clicked().connect(SLOT(this, ProfilePanel::addProfileButtonClicked));
        topToolBar()->addSeparator();
        editProfileButton = topToolBar()->addButton("Edit Output Profile");
        editProfileButton->setIcon("icons/edit-icon.png");
        editProfileButton ->clicked().connect(SLOT(this, ProfilePanel::editSelectedProfile));
        editProfileButton->setEnabled(false);
        topToolBar()->addSeparator();
        removeProfileButton = topToolBar()->addButton("Remove Output Profile");
        removeProfileButton->setIcon("icons/remove-icon.png");
        removeProfileButton ->clicked().connect(SLOT(this, ProfilePanel::removeSelectedProfile));
        removeProfileButton->setEnabled(false);
      }

      void ProfilePanel::setProject(Ptr<db::Project> p){
        _profile_table->setPresets(p->presets().get().all());
        _project=p;
      }

      void ProfilePanel::addProfileButtonClicked(){
        _profile_chooser=Ptr<ProfileChooser>(new ProfileChooser());
        _profile_chooser->selected.connect(SLOT(this,ProfilePanel::profileChooserSelected));
        _profile_chooser->show();
      }

      void ProfilePanel::removeSelectedProfile(){
        std::string pfile=boost::any_cast<string > (_profile_table->model()->data(_profile_table->selectedRows()[0], 0)).c_str();
        LOGDEBUG("PresetFilename="<<pfile);
        db::HiveDb db=org::esb::hive::DatabaseService::getDatabase();
        db::Preset preset=litesql::select<db::Preset>(db, db::Preset::Filename==pfile).one();
        _project->presets().unlink(preset);
        _profile_table->setPresets(_project->presets().get().all());
        removeProfileButton->setEnabled(false);
        editProfileButton->setEnabled(false);
      }

      void ProfilePanel::editSelectedProfile(){
        std::string pfile=boost::any_cast<string > (_profile_table->model()->data(_profile_table->selectedRows()[0], 0)).c_str();
        LOGDEBUG("PresetFilename="<<pfile);

        _profile_editor=new PresetsEditorWindow(pfile);
        //_profile_editor->saved.connect(SLOT(this, ProfilePanel::profileSaved));
        _profile_editor->show();
        
      }
      void ProfilePanel::profileSaved(){
        //_profile->update();
      }

      void ProfilePanel::enableButtons(){
        if(_profile_table->selectedRows().size()>0){
        removeProfileButton->setEnabled(true);
        editProfileButton->setEnabled(true);
        }
      }

      void ProfilePanel::profileChooserSelected(){
        std::string p=_profile_chooser->getSelectedProfile();
        org::esb::hive::PresetReader reader(p);
        db::Preset preset(_project->getDatabase());
        preset.filename=p;
        preset.name=reader.getPreset()["name"];
        preset.update();
        _project->presets().link(preset);
        _profile_table->setPresets(_project->presets().get().all());
        /*
        int c=_profile_chooser->getSelectedProfileId();
        db::HiveDb db=org::esb::hive::DatabaseService::getDatabase();
        db::Profile profile=litesql::select<db::Profile>(db, db::Profile::Id==c).one();
        if(_project)
          _project->profiles().link(profile);
        _profile_table->setProfiles(_project->profiles().get().all());
         */
        removeProfileButton->setEnabled(false);
        editProfileButton->setEnabled(false);
        
      }
    }
  }
}
