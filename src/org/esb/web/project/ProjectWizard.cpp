
#include "org/esb/db/hivedb.hpp"
#include "ProjectWizard.h"

#include "Wt/WBorderLayout"
#include "Wt/WText"
#include "Wt/WFitLayout"
#include "Wt/Ext/Panel"
#include "Wt/Ext/Button"



#include "org/esb/sql/Connection.h"
#include "org/esb/sql/PreparedStatement.h"
#include "org/esb/config/config.h"
namespace org {
  namespace esb {
    namespace web {
      ProjectWizard::ProjectWizard() : Wt::Ext::Dialog("Project Editor") {
        _project_id=0;
        resize(900, 600);
        setBorder(false);
        //setSizeGripEnabled(true);

        Wt::WBorderLayout *l = new Wt::WBorderLayout();

        setLayout(l);

        layout()->setContentsMargins(0, 0, 0, 0);

        _db=Ptr<db::HiveDb>(new db::HiveDb("mysql",org::esb::config::Config::getProperty("db.url")));
        //        Wt::Ext::Panel * center = new InputFilePanel();
        _filePanel=Ptr<InputFilePanel>(new InputFilePanel());


        //center->setLayout(new Wt::WFitLayout());
        //center->layout()->addWidget(new Wt::WText("Center"));
        //center->resize(300,300);

        ((Wt::WBorderLayout*)layout())->addWidget(_filePanel.get(), Wt::WBorderLayout::Center);

        Wt::Ext::Panel * south_panel = new Wt::Ext::Panel();
        south_panel->setLayout(new Wt::WFitLayout());
        south_panel->layout()->addWidget(new Wt::WText("South"));
        south_panel->resize(Wt::WLength(), Wt::WLength(50,Wt::WLength::Percentage));
        south_panel->setCollapsible(true);
        south_panel->setAnimate(true);
        south_panel->setResizable(true);
        ((Wt::WBorderLayout*)layout())->addWidget(south_panel, Wt::WBorderLayout::South);


        addButton(new Wt::Ext::Button("Cancel"));
        buttons().back()->clicked.connect(SLOT(this, ProjectWizard::cancel));
        addButton(new Wt::Ext::Button("Save"));
        buttons().back()->clicked.connect(SLOT(this, ProjectWizard::save));
        /*
        ((Wt::WBorderLayout*)layout())->addWidget(new Wt::WText("North"), Wt::WBorderLayout::North);

        ((Wt::WBorderLayout*)layout())->addWidget(new Wt::WText("East"), Wt::WBorderLayout::East);
        ((Wt::WBorderLayout*)layout())->addWidget(new Wt::WText("West"), Wt::WBorderLayout::West);
        */
      }

      void ProjectWizard::open() {
        open(Ptr<db::Project>(new db::Project(*_db.get())));
      }

      void ProjectWizard::open(Ptr<db::Project> p) {
        _project=p;
        _project->update();
        _filePanel->setProject(_project);
        this->show();
      }

      void ProjectWizard::save() {
        LOGDEBUG("Project save with id:"<<_project->id)
          _project->update();
        LOGDEBUG("Project saved:" << _project->id)
          this->done(Accepted);
      }

      void ProjectWizard::cancel() {

        _project->del();
        this->done(Rejected);
      }

    }
  }
}