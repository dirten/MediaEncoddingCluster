
#include "org/esb/db/hivedb.hpp"
#include "Wt/Ext/MessageBox"
#include "ProjectWizard.h"

#include "Wt/WBorderLayout"
#include "Wt/WText"
#include "Wt/WFitLayout"
#include "PropertyPanel.h"
#include "Wt/Ext/Panel"
#include "Wt/Ext/Button"
#include "Wt/Ext/MessageBox"
#include "Wt/Ext/ToolBar"




#include "org/esb/config/config.h"
#include "PresetTree.h"
#include "org/esb/web/project/InputFilePanel.h"
#include "org/esb/hive/JobUtil.h"
#include "org/esb/hive/DatabaseService.h"
/*on windows there is a macro defined with name MessageBox*/
#ifdef MessageBox
#undef MessageBox
#endif
namespace org {
  namespace esb {
    namespace web {
      namespace v2 {

        ProjectWizard::ProjectWizard() : Wt::Ext::Dialog() {
          LOGDEBUG("ProjectWizard::ProjectWizard() :"<<this);
          _project_id = 0;
          resize(900, 650);
          setBorder(false);
          setTitle("Project Editor");
          //setSizeGripEnabled(true);

          Wt::WBorderLayout *l = new Wt::WBorderLayout();

          setLayout(l);

          layout()->setContentsMargins(0, 0, 0, 0);

          _db = Ptr<db::HiveDb > (new db::HiveDb(org::esb::hive::DatabaseService::getDatabase()));

          //          ((Wt::WBorderLayout*)layout())->addWidget(new org::esb::web::InputFilePanel(), Wt::WBorderLayout::Center);
//          ((Wt::WBorderLayout*)layout())->addWidget(new org::esb::web::InputFilePanel(), Wt::WBorderLayout::Center);
          ((Wt::WBorderLayout*)layout())->addWidget(new PropertyPanel(), Wt::WBorderLayout::Center);
//          ((Wt::Ext::Panel*)((Wt::WBorderLayout*)layout())->widgetAt(Wt::WBorderLayout::East))->resize(470, Wt::WLength());
//          ((Wt::Ext::Panel*)((Wt::WBorderLayout*)layout())->widgetAt(Wt::WBorderLayout::East))->setResizable(true);

          setBottomToolBar(new Wt::Ext::ToolBar());
          Wt::Ext::Button*button=NULL;
          button= bottomToolBar()->addButton("Cancel");
          button->clicked().connect(SLOT(this, ProjectWizard::cancel));
          button->setIcon("icons/remove-icon.png");
          button=bottomToolBar()->addButton("Save");
          button->clicked().connect(SLOT(this, ProjectWizard::save));
          button->setIcon("icons/accept-icon.png");
          button=bottomToolBar()->addButton("Save & start Encoding");
          button->clicked().connect(SLOT(this, ProjectWizard::save_and_start));
          button->setIcon("icons/process-accept-icon.png");
        }

        void ProjectWizard::refresh() {
          LOGDEBUG("void ProjectWizard::refresh()");
        }

        ProjectWizard::~ProjectWizard() {
          LOGDEBUG("ProjectWizard::~ProjectWizard() :"<<this);
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
          ((PropertyPanel*) ((Wt::WBorderLayout*)layout())->widgetAt(Wt::WBorderLayout::Center))->setProject(_project);
//          ((org::esb::web::InputFilePanel*)((Wt::WBorderLayout*)layout())->widgetAt(Wt::WBorderLayout::Center))->setProject(_project);
          this->show();
        }

        void ProjectWizard::save() {
          LOGDEBUG("Project save with id:" << _project->id)
          ((PropertyPanel*) ((Wt::WBorderLayout*)layout())->widgetAt(Wt::WBorderLayout::Center))->save();
          _project->update();
          LOGDEBUG("Project saved:" << _project->id)
          saved.emit();
          this->done(Accepted);
        }

        void ProjectWizard::save_and_start() {
          LOGDEBUG("Project save with id:" << _project->id);
          ((PropertyPanel*) ((Wt::WBorderLayout*)layout())->widgetAt(Wt::WBorderLayout::Center))->save();
          if (_project->outdirectory.value().length() <= 0) {
            Wt::Ext::MessageBox *box = new Wt::Ext::MessageBox("Missing Output Directory", "please define an output Directory in the Project Properties", Wt::Warning, Wt::Ok);
            box->show();
          } else if (_project->presets().get().count() <= 0) {
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
}

