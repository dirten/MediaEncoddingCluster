#include "SqlTable.h"
#include "SqlTableModel.h"
#include "Wt/Ext/LineEdit"
#include "org/esb/sql/Connection.h"
#include "org/esb/sql/Statement.h"
#include "org/esb/sql/ResultSet.h"

#include "org/esb/config/config.h"
using namespace org::esb;
using namespace org::esb::config;
namespace org{
namespace esb{
namespace web{
SqlTable::SqlTable(std::string sql, Wt::WContainerWidget * parent):Wt::Ext::TableView(parent){
  sql::Connection con(Config::getProperty("db.connection"));
//  if(sql.size()==0)sql.append("select ")
  sql::Statement stmt=con.createStatement(sql.c_str());
  mod=new SqlTableModel(stmt.executeQuery(), (Wt::WContainerWidget *)this);
  setModel(mod);
  setAlternatingRowColors(true);
  resizeColumnsToContents(true);
  setHighlightMouseOver(true);
  setSelectionBehavior(Wt::SelectRows);
  setSelectionMode(Wt::SingleSelection);
//  setEditor(2,new Wt::Ext::LineEdit());
  int colc=model()->columnCount();
  for(int a=0;a<colc;a++){
    enableColumnHiding(a, true);
    setColumnSortable(a, true);
  }
//  setDataLocation(Wt::Ext::ServerSide);
//  setPageSize(10);
//      table->setBottomToolBar(table->createPagingToolBar());
//  setTopToolBar(createPagingToolBar());
//  topToolBar()->addButton("test");
  
}
void SqlTable::reload(string sql){
  ((SqlTableModel*)model())->clear();
  sql::Connection con(Config::getProperty("db.connection"));
  sql::Statement stmt=con.createStatement(sql.c_str());
  mod=new SqlTableModel(stmt.executeQuery());
  setModel(mod);

//  ((SqlTableModel*)model())->load(stmt.executeQuery());
}
}}}


