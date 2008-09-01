#include <Wt/WContainerWidget>
#include <Wt/WStandardItemModel>
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


#include "SqlTable.h"
using namespace org::esb;
using namespace org::esb::config;

namespace org{
namespace esb{
namespace web{

class Files: public Wt::WContainerWidget{
  public:
    Files(Wt::WContainerWidget * parent=0):Wt::WContainerWidget(parent){
      tab = new SqlTable(std::string("select * from files"),this);
      tab->itemSelectionChanged.connect(SLOT(this, Files::cellClicked));
      str=new SqlTable("select * from streams limit 2", this);
//      str->resize(1000,400);
      
    }
    
    void cellClicked(){
//      boost::any_cast<string>(tab->model()->data(tab->currentRow(),1));
      std::string sql="select * from streams where fileid="+boost::any_cast<string>(tab->model()->data(tab->selectedRows()[0],0));
      std::cout << "Clicked"<<tab->selectedRows()[0]<<boost::any_cast<string>(tab->model()->data(tab->selectedRows()[0],0))<<endl;
      std::cout << "Sql="<<sql<<std::endl;
//      if(str)delete str;
//      str->reload(sql);
    }
    private:
      SqlTable * tab;
      SqlTable * str;
};
}}}

