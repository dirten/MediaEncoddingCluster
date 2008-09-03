#include <Wt/WWidget>
#include <Wt/WLength>
#include <Wt/WContainerWidget>
#include <Wt/Ext/Button>
#include "SqlTable.h"
#include "ProfilesEdit.cpp"
namespace org{
namespace esb{
namespace web{
class Profiles: public Wt::WContainerWidget{
  public:
    Profiles(Wt::WContainerWidget * parent=0):Wt::WContainerWidget(parent) {
      t=new SqlTable(std::string("select * from profiles"), this);
      t->resize(Wt::WLength(),300);
      t->itemSelectionChanged.connect(SLOT(this, Profiles::enableButton));

//      button=t->topToolBar()->addButton("Edit selected Profile");
//      button->setEnabled(false);
//	  button->clicked.connect(SLOT(this, Profiles::editProfile));
	  edit=new ProfilesEdit(this);
      edit->resize(500,Wt::WLength());
//	  addWidget(edit);
    }
    void enableButton(){
		int d=atoi(boost::any_cast<string>(t->model()->data(t->selectedRows()[0],0)).c_str());
    	edit->setData(d);
//      button->setEnabled(true);
    }
    void editProfile(){
    	edit->setData(1);
//		((Wt::WStackedWidget*)parent())->setCurrentIndex(4);
//      button->setEnabled(true);
    }
  private:
  	Wt::Ext::Button * button;
  	SqlTable * t;
  	ProfilesEdit * edit;
};
}}}

