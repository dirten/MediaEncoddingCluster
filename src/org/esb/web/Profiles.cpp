#include <Wt/WContainerWidget>
#include <Wt/Ext/TableView>
#include <Wt/WFitLayout>


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


#include "org/esb/config/config.h"
#include "org/esb/sql/Connection.h"
#include "org/esb/sql/Statement.h"

using namespace org::esb;
using namespace org::esb::config;
namespace org{
namespace esb{
namespace web{


class Profiles: public Wt::WContainerWidget{
  public:
    Profiles():Wt::WContainerWidget(0) {
//      setLayout(new Wt::WFitLayout());
//      setCollapsible(true);
//      setResizable(true);
      table=new Wt::Ext::TableView();
//      table->setAutoScrollBars(true);
      sql::Connection con(Config::getProperty("db.connection"));
      sql::Statement stmt=con.createStatement("select * from profiles");
      table->setModel(new SqlTableModel(stmt.executeQuery(), this));
//      table->setLayout(new Wt::WFitLayout());
      table->setAlternatingRowColors(true);
      table->resizeColumnsToContents(true);
//      table->setMinimumSize(400,400);
      table->resize(1000, 400);
      table->setHighlightMouseOver(true);
      int colc=table->model()->columnCount();
      for(int a=0;a<colc;a++){
        table->enableColumnHiding(a, true);
        table->setColumnSortable(a, true);
      }

      table->setDataLocation(Wt::Ext::ServerSide);
      table->setPageSize(10);
//      table->setBottomToolBar(table->createPagingToolBar());
      table->setTopToolBar(table->createPagingToolBar());
	  table->topToolBar()->addButton("test");
//	  table->setBottomToolBar(table->bottomToolBar());
      addWidget(table);
    }
    Wt::WWidget * home(){
      return this;
    }
    private:
      Wt::Ext::TableView * table;
};
}}}

