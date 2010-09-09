
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

/*on windows there is a macro defined with name MessageBox*/
#ifdef MessageBox
#undef MessageBox
#endif
namespace org {
  namespace esb {
    namespace web {
      namespace v2 {

        ProjectWizard::ProjectWizard() : Wt::Ext::Dialog("Project Editor") {
          _project_id = 0;
          resize(1000, 550);
          setBorder(false);
          //setSizeGripEnabled(true);

          Wt::WBorderLayout *l = new Wt::WBorderLayout();

          setLayout(l);

          layout()->setContentsMargins(0, 0, 0, 0);

          _db = Ptr<db::HiveDb > (new db::HiveDb("mysql", org::esb::config::Config::getProperty("db.url")));



          addButton(new Wt::Ext::Button("Cancel"));
          buttons().back()->clicked.connect(SLOT(this, ProjectWizard::cancel));
          buttons().back()->setIcon("icons/remove-icon.png");
          addButton(new Wt::Ext::Button("Save"));
          buttons().back()->clicked.connect(SLOT(this, ProjectWizard::save));
          buttons().back()->setIcon("icons/accept-icon.png");
          addButton(new Wt::Ext::Button("Save & start Encoding"));
          buttons().back()->clicked.connect(SLOT(this, ProjectWizard::save_and_start));
          buttons().back()->setIcon("icons/process-accept-icon.png");
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
          this->show();
        }

        void ProjectWizard::save() {
          LOGDEBUG("Project save with id:" << _project->id)
          _project->update();
          LOGDEBUG("Project saved:" << _project->id)
          saved.emit();
          this->done(Accepted);
        }

        void ProjectWizard::save_and_start() {
          LOGDEBUG("Project save with id:" << _project->id);
          _project->update();
          LOGDEBUG("Project saved:" << _project->id);
          
          Wt::Ext::MessageBox *box = new Wt::Ext::MessageBox("Encodings successfull created", "Encodings successfull created<br/>please look in the \"Encodings->All Encodings\"  Menu<br/>to see the running Encodings", Wt::Warning, Wt::Ok);
          box->show();

          saved.emit();
          this->done(Accepted);
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

