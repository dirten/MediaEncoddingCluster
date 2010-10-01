
#include "org/esb/db/hivedb.hpp"
#include "Wt/Ext/MessageBox"
#include "ProjectWizard.h"

#include "Wt/WBorderLayout"
#include "Wt/WText"
#include "Wt/WFitLayout"
#include "Wt/Ext/Panel"
#include "Wt/Ext/Button"
#include "Wt/Ext/MessageBox"




#include "org/esb/config/config.h"

#include "org/esb/hive/JobUtil.h"
#include "org/esb/hive/DatabaseService.h"
/*on windows there is a macro defined with name MessageBox*/
#ifdef MessageBox
#undef MessageBox
#endif
namespace org {
  namespace esb {
    namespace web {

      ProjectWizard::ProjectWizard() : Wt::Ext::Dialog("Project Editor") {
        _project_id = 0;
        resize(1000, 550);
        setBorder(false);
        //setSizeGripEnabled(true);

        Wt::WBorderLayout *l = new Wt::WBorderLayout();

        setLayout(l);

        layout()->setContentsMargins(0, 0, 0, 0);

        _db = Ptr<db::HiveDb > (new db::HiveDb(org::esb::hive::DatabaseService::getDatabase()));
        //_db->verbose=true;
        _filePanel = Ptr<InputFilePanel > (new InputFilePanel());

        _profilePanel = Ptr<ProfilePanel > (new ProfilePanel());
        _profilePanel->resize(450, Wt::WLength());
        _profilePanel->setResizable(true);

        _filterPanel = Ptr<FilterPanel > (new FilterPanel());
        _filterPanel->resize(450, 200);
        _filterPanel->setResizable(true);

        _propertyPanel = Ptr<ProjectPropertyPanel > (new ProjectPropertyPanel());
        _propertyPanel->resize(450, 200);
        _propertyPanel->setResizable(true);



        //center->setLayout(new Wt::WFitLayout());
        //center->layout()->addWidget(new Wt::WText("Center"));
        //center->resize(300,300);

        ((Wt::WBorderLayout*)layout())->addWidget(_filePanel.get(), Wt::WBorderLayout::Center);
        ((Wt::WBorderLayout*)layout())->addWidget(_profilePanel.get(), Wt::WBorderLayout::East);
        //        ((Wt::WBorderLayout*)layout())->addWidget(_filterPanel.get(), Wt::WBorderLayout::South);

        Wt::Ext::Panel * south_panel = new Wt::Ext::Panel();
        south_panel->setLayout(new Wt::WBorderLayout());
        ((Wt::WBorderLayout*)south_panel->layout())->addWidget(_filterPanel.get(), Wt::WBorderLayout::Center);
        ((Wt::WBorderLayout*)south_panel->layout())->addWidget(_propertyPanel.get(), Wt::WBorderLayout::East);
        south_panel->resize(Wt::WLength(), 200);
        south_panel->setAnimate(true);
        south_panel->setResizable(true);
        ((Wt::WBorderLayout*)layout())->addWidget(south_panel, Wt::WBorderLayout::South);


        addButton(new Wt::Ext::Button("Cancel"));
        buttons().back()->clicked().connect(SLOT(this, ProjectWizard::cancel));
        buttons().back()->setIcon("icons/remove-icon.png");
        addButton(new Wt::Ext::Button("Save"));
        buttons().back()->clicked().connect(SLOT(this, ProjectWizard::save));
        buttons().back()->setIcon("icons/accept-icon.png");
        addButton(new Wt::Ext::Button("Save & start Encoding"));
        buttons().back()->clicked().connect(SLOT(this, ProjectWizard::save_and_start));
        buttons().back()->setIcon("icons/process-accept-icon.png");
        /*
        ((Wt::WBorderLayout*)layout())->addWidget(new Wt::WText("North"), Wt::WBorderLayout::North);

        ((Wt::WBorderLayout*)layout())->addWidget(new Wt::WText("East"), Wt::WBorderLayout::East);
        ((Wt::WBorderLayout*)layout())->addWidget(new Wt::WText("West"), Wt::WBorderLayout::West);
         */
      }

      ProjectWizard::~ProjectWizard() {
        LOGDEBUG("ProjectWizard::~ProjectWizard() ");
      }

      void ProjectWizard::open(int pid) {
        try {
          db::Project project = litesql::select<db::Project > (*_db.get(), db::Project::Id == pid).one();
          open(Ptr<db::Project > (new db::Project(project)));
        } catch (litesql::NotFound & ex) {
          LOGERROR("Project with id " << pid << " could not be loaded!" << ex.what());
        }
      }

      void ProjectWizard::open() {
        open(Ptr<db::Project > (new db::Project(*_db.get())));
      }

      void ProjectWizard::open(Ptr<db::Project> p) {
        _project = p;
        _project->update();
        _filePanel->setProject(_project);
        _profilePanel->setProject(_project);
        _filterPanel->setProject(_project);
        _propertyPanel->setProject(_project);
        this->show();
      }

      void ProjectWizard::save() {
        LOGDEBUG("Project save with id:" << _project->id)
        _propertyPanel->save();
        _project->update();
        LOGDEBUG("Project saved:" << _project->id)
        saved.emit();
        this->done(Accepted);
      }

      void ProjectWizard::save_and_start() {
        LOGDEBUG("Project save with id:" << _project->id);
        _propertyPanel->save();
        if (_project->outdirectory.value().length() <= 0) {
          Wt::Ext::MessageBox *box = new Wt::Ext::MessageBox("Missing Output Directory", "please define an output Directory in the Project Properties", Wt::Warning, Wt::Ok);
          box->show();
        } else if (_project->profiles().get().count() <= 0) {
          Wt::Ext::MessageBox *box = new Wt::Ext::MessageBox("Missing Output Profile", "please add an output Profile in the Output Profiles Panel ", Wt::Warning, Wt::Ok);
          box->show();
        } else if (_project->mediafiles().get().count() <= 0) {
          Wt::Ext::MessageBox *box = new Wt::Ext::MessageBox("Missing Input Files", "please add an Input Video in the Input Videos Panel ", Wt::Warning, Wt::Ok);
          box->show();
        } else {
          _project->update();
          LOGDEBUG("Project saved:" << _project->id);
          org::esb::hive::JobUtil::createJob(_project);
          Wt::Ext::MessageBox *box = new Wt::Ext::MessageBox("Encodings successfull created", "Encodings successfull created<br/>please look in the \"Encodings->All Encodings\"  Menu<br/>to see the running Encodings", Wt::Warning, Wt::Ok);
          box->show();

          saved.emit();
          this->done(Accepted);
        }
      }

      void ProjectWizard::cancel() {

        //        _project->del();
        canceled.emit();
        this->done(Rejected);
      }

    }
  }
}


