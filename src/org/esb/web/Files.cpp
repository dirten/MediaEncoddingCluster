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
using namespace org::esb;
using namespace org::esb::config;

namespace org{
namespace esb{
namespace web{

class Files: public Wt::WContainerWidget{
  public:
    Files():Wt::WContainerWidget(0){
//      setLayout(new Wt::WFitLayout());
      table=new Wt::Ext::TableView();
//      table->setAutoScrollBars(true);
      sql::Connection con(Config::getProperty("db.connection"));
      sql::Statement stmt=con.createStatement("select * from files");
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
      table->setTopToolBar(table->createPagingToolBar());
      addWidget(table);



      timer=new Wt::WTimer();
      timer->setInterval(3000);
      timer->timeout.connect(SLOT(this,Files::removeLastTest));
//      timer->start();
//      table->model()->removeRow(1);
//      table->repaint();
//      table->bottomToolBar()->addSeparator();
//      table->bottomToolBar()->addButton("Other button");

//      boost::thread t(boost::bind(&Files::removeLastTest, this));
    }
    Wt::WWidget * home(){
      return this;
    }
    
    void removeLastTest(){
      if(table->model()->rowCount()>0){
        table->model()->removeRow(0);
      }else{
        timer->stop();
      }
    }
    
    void reload(){
      sql::Connection con(Config::getProperty("db.connection"));
      sql::Statement stmt=con.createStatement("select * from files");
      table->setModel(new SqlTableModel(stmt.executeQuery(), this));
      table->refresh();
    }
    private:
      Wt::Ext::TableView * table;
      Wt::WTimer * timer;
};
}}}

