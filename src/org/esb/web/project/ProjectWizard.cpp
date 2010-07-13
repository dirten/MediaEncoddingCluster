

#include "ProjectWizard.h"

#include "Wt/WBorderLayout"
#include "Wt/WText"
#include "Wt/WFitLayout"
#include "Wt/Ext/Panel"
#include "Wt/Ext/Button"
#include "InputFilePanel.h"


#include "org/esb/sql/Connection.h"
#include "org/esb/sql/PreparedStatement.h"
#include "org/esb/config/config.h"
namespace org {
  namespace esb {
    namespace web {

      ProjectWizard::ProjectWizard() : Wt::Ext::Dialog("Project Wizard") {
        _project_id=0;
        resize(900, 700);
        setBorder(false);
        //setSizeGripEnabled(true);

        Wt::WBorderLayout *l = new Wt::WBorderLayout();
        
        setLayout(l);

        layout()->setContentsMargins(0, 0, 0, 0);


        Wt::Ext::Panel * center = new InputFilePanel();



        //center->setLayout(new Wt::WFitLayout());
        //center->layout()->addWidget(new Wt::WText("Center"));
        //center->resize(300,300);

        ((Wt::WBorderLayout*)layout())->addWidget(center, Wt::WBorderLayout::Center);

        Wt::Ext::Panel * south_panel = new Wt::Ext::Panel();
        south_panel->setLayout(new Wt::WFitLayout());
        south_panel->layout()->addWidget(new Wt::WText("South"));
        south_panel->resize(Wt::WLength(), 50);
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
        this->show();
      }

      void ProjectWizard::save() {
        LOGDEBUG("Project save with id:"<<_project_id)
        org::esb::sql::Connection con(org::esb::config::Config::getProperty("db.connection"));
        org::esb::sql::PreparedStatement pstmt=con.prepareStatement("REPLACE INTO project (id,name) values (:id,:name)");
        pstmt.setInt("id",_project_id);
        pstmt.setString("name","dummy name");
        pstmt.execute();
        _project_id=pstmt.getLastInsertId();
        LOGDEBUG("Project saved:" << _project_id)
        this->accept();
      }

      void ProjectWizard::cancel() {
        this->reject();
      }

    }
  }
}