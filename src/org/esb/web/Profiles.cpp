#include <Wt/WWidget>
#include <Wt/WLength>
#include <Wt/WContainerWidget>
#include <Wt/WFitLayout>
#include <Wt/Ext/Button>
#include <Wt/Ext/ToolBar>
#include <Wt/Ext/Dialog>
#include "SqlTable.h"
#include "ProfilesForm.h"
namespace org {
  namespace esb {
    namespace web {

      class Profiles : public Wt::Ext::Container {
      public:

        Profiles(Wt::WContainerWidget * parent = 0) : Wt::Ext::Container() {
          Wt::WFitLayout * l = new Wt::WFitLayout();
          setLayout(l);
          t = new SqlTable(std::string("select * from profiles"));
          t->itemSelectionChanged.connect(SLOT(this, Profiles::enableButton));
          t->setTopToolBar(new Wt::Ext::ToolBar());
          layout()->addWidget(t);

          buttonEdit = t->topToolBar()->addButton("Edit selected Profile");
          buttonEdit->setEnabled(false);
          buttonEdit->clicked.connect(SLOT(this, Profiles::editProfile));
          t->topToolBar()->addSeparator();
          buttonDelete = t->topToolBar()->addButton("Delete selected Profile");
          buttonDelete->setEnabled(false);
          buttonDelete->clicked.connect(SLOT(this, Profiles::deleteProfile));
        }

        void enableButton() {
          if (t->selectedRows().size() > 0) {
            int d = atoi(boost::any_cast<string > (t->model()->data(t->selectedRows()[0], 0)).c_str());
            buttonEdit->setEnabled(true);
          }
        }

        void reloadProfiles() {
          t->reload("select * from profiles");
        }

        void deleteProfile() {
          /*confirm deleting*/
        }

        void editProfile() {
          int c = atoi(boost::any_cast<string > (t->model()->data(t->selectedRows()[0], 0)).c_str());
          d = new Wt::Ext::Dialog("Profile");
          d->resize(500, 400);
          ProfilesForm * pf = new ProfilesForm(d->contents());
          pf->profileSaved.connect(SLOT(this, Profiles::profileSaved));
          pf->profileCanceled.connect(SLOT(d, Wt::Ext::Dialog::accept));
          pf->setProfile(c);
          d->show();
        }
        void profileSaved(){
          t->reload("select * from profiles");
          delete d;
        }
      private:
        Wt::Ext::Button * buttonEdit;
        Wt::Ext::Button * buttonNew;
        Wt::Ext::Button * buttonDelete;
        SqlTable * t;
        Wt::Ext::Dialog * d;
        //        ProfilesEdit * edit;
      };
    }
  }
}

