#include <Wt/WWidget>
#include <Wt/WLength>
#include <Wt/WContainerWidget>
#include <Wt/WFitLayout>
#include <Wt/Ext/Button>
#include <Wt/Ext/ToolBar>
#include "SqlTable.h"
#include "ProfilesEdit.cpp"
namespace org {
  namespace esb {
    namespace web {

      class Profiles : public Wt::Ext::Container {
      public:

        Profiles(Wt::WContainerWidget * parent = 0) : Wt::Ext::Container() {
          //      this->resize(Wt::WLength(100, Wt::WLength::Percentage), Wt::WLength(100, Wt::WLength::Percentage));
          Wt::WFitLayout * l=new Wt::WFitLayout();
          setLayout(l);
          t = new SqlTable(std::string("select * from profiles"));
          t->itemSelectionChanged.connect(SLOT(this, Profiles::enableButton));
//          t->resize(Wt::WLength(), 300);

//          t->setTopToolBar(t->createPagingToolBar());
          t->setTopToolBar(new Wt::Ext::ToolBar());
          layout()->addWidget(t);

          //	  t->resize(800,300);
          buttonEdit = t->topToolBar()->addButton("Edit selected Profile");
          buttonEdit->setEnabled(false);
          buttonEdit->clicked.connect(SLOT(this, Profiles::editProfile));

//          buttonNew = t->topToolBar()->addButton("Create new Profile");
          //      buttonNew->setEnabled(false);
//          buttonNew->clicked.connect(SLOT(this, Profiles::newProfile));
           
/*
          edit = new ProfilesEdit();
          //      edit->resize(500,Wt::WLength());
          edit->setEnabled(false);
          edit->profileSaved.connect(SLOT(this, Profiles::reloadProfiles));
          layout()->addWidget(edit);
*/
        }

        void enableButton() {
          if (t->selectedRows().size() > 0) {
            int d = atoi(boost::any_cast<string > (t->model()->data(t->selectedRows()[0], 0)).c_str());
  //          edit->setData(d);
            buttonEdit->setEnabled(true);
  //          edit->setEnabled(false);
          }
        }

        void reloadProfiles() {
          t->reload("select * from profiles");
        }

        void editProfile() {
          //    	edit->setData(1);
//          edit->setEnabled(true);
          //		((Wt::WStackedWidget*)parent())->setCurrentIndex(4);
          //      button->setEnabled(true);
        }

        void newProfile() {
//          edit->setData(0);
          //    	edit->setData(1);
//          edit->setEnabled(true);
          //		((Wt::WStackedWidget*)parent())->setCurrentIndex(4);
          //      button->setEnabled(true);
        }
      private:
        Wt::Ext::Button * buttonEdit;
        Wt::Ext::Button * buttonNew;
        SqlTable * t;
//        ProfilesEdit * edit;
      };
    }
  }
}

