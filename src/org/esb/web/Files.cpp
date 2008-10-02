#include <Wt/WContainerWidget>
/*#include <Wt/WStandardItemModel>
#include <Wt/WFitLayout>
#include <Wt/WTimer>
#include <Wt/WBorderLayout>
#include <Wt/WString>
#include <Wt/Ext/TableView>
#include <Wt/WContainerWidget>
#include <Wt/Ext/ToolBar>
#include "org/esb/config/config.h"
#include "org/esb/lang/Thread.h"
#include "org/esb/sql/Connection.h"
#include "org/esb/sql/Statement.h"
#include "org/esb/sql/ResultSet.h"
#include "org/esb/sql/ResultSetMetaData.h"

#include <iostream>
#include "SqlTableModel.h"
*/

#include <Wt/WLength>

#include "SqlTable.h"
#include "FileInfo.cpp"
using namespace org::esb;
//using namespace org::esb::config;

namespace org{
namespace esb{
namespace web{

class Files: public Wt::WContainerWidget{
  public:
    Files(Wt::WContainerWidget * parent=0):Wt::WContainerWidget(parent){
      tab = new SqlTable(std::string("select * from files where parent=0"),this);
      tab->resize(Wt::WLength()/*100,Wt::WLength::Percentage)*/,300);
      tab->itemSelectionChanged.connect(SLOT(this, Files::cellClicked));
      str=new SqlTable("select * from files limit 0 ", this);
      str->resize(Wt::WLength()/*100,Wt::WLength::Percentage)*/,100);
//      str->resize(1000,400);
      
      info=new FileInfo(this);
    }
    
    void cellClicked(){
//      boost::any_cast<string>(tab->model()->data(tab->currentRow(),1));
      std::string idstr=boost::any_cast<string>(tab->model()->data(tab->selectedRows()[0],0));
      std::string sql="select * from files where parent="+idstr;
//      std::cout << "Clicked"<<tab->selectedRows()[0]<<boost::any_cast<string>(tab->model()->data(tab->selectedRows()[0],0))<<endl;
//      std::cout << "Sql="<<sql<<std::endl;
//      if(str)delete str;
      str->reload(sql);
      info->setData(atoi(idstr.c_str()));
    }
    private:
      SqlTable * tab;
      SqlTable * str;
      FileInfo * info;
};
}}}

