#include <Wt/WWidget>
#include <Wt/WContainerWidget>
//#include <Wt/Ext/TableView>
//#include <Wt/WFitLayout>


//#include <Wt/WContainerWidget>
//#include <Wt/WStandardItemModel>
//#include <Wt/WFitLayout>
//#include <Wt/WTimer>
//#include <Wt/WBorderLayout>
//#include <Wt/WString>
//#include <Wt/Ext/TableView>
//#include <Wt/WContainerWidget>
//#include <Wt/Ext/ToolBar>
//#include "org/esb/config/config.h"
//#include "org/esb/lang/Thread.h"
//#include "org/esb/sql/Connection.h"
//#include "org/esb/sql/Statement.h"
//#include "org/esb/sql/ResultSet.h"
//#include "org/esb/sql/ResultSetMetaData.h"


//#include "org/esb/config/config.h"
//#include "org/esb/sql/Connection.h"
//#include "org/esb/sql/Statement.h"

#include "SqlTable.h"
using namespace org::esb;
//using namespace org::esb::config;
namespace org{
namespace esb{
namespace web{


class Profiles: public Wt::WContainerWidget{
  public:
    Profiles(Wt::WContainerWidget * parent=0):Wt::WContainerWidget(parent) {
      new SqlTable(std::string("select * from profiles"), this);
    }
};
}}}

