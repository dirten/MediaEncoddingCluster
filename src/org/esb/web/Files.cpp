#include <Wt/WContainerWidget>
#include <Wt/WStandardItemModel>
#include <Wt/WFitLayout>
#include <Wt/Ext/TableView>
#include "org/esb/config/config.h"
#include "org/esb/sql/Connection.h"
#include "org/esb/sql/Statement.h"
#include "org/esb/sql/ResultSet.h"
#include "org/esb/sql/ResultSetMetaData.h"

#include <iostream>
using namespace org::esb;
using namespace org::esb::config;

namespace org{
namespace esb{
namespace web{
class SqlTableModel:public Wt::WStandardItemModel{
  public:
    SqlTableModel(sql::ResultSet result,Wt::WObject* parent):Wt::WStandardItemModel(0,0, parent){
      sql::ResultSetMetaData * rsmd=result.getResultSetMetaData();
      int fieldCount=rsmd->getColumnCount();
      insertColumns(0,fieldCount);
      for(int a=0;result.next();a++){
        if(rowCount()<=a)
          insertRow(rowCount());
        for(int b=0;b<fieldCount;b++)
          if(!setData(a,b,result.getString(b))){
            std::cout<<"set data Failed"<<std::endl;
          }
      }

      for(int a=0;a<fieldCount;a++){
        if(!setHeaderData(a,rsmd->getColumnName(a))){
          std::cout<<"Failed"<<std::endl;
        }
      }
    }
};

class Files: public Wt::WContainerWidget{
  public:
    Files():Wt::WContainerWidget(0){
      sql::Connection con(Config::getProperty("db.connection"));
      sql::Statement stmt=con.createStatement("select * from files");
      Wt::Ext::TableView * table=new Wt::Ext::TableView(this);
      table->setModel(new SqlTableModel(stmt.executeQuery(), this));
      
      table->setAlternatingRowColors(true);
      table->resizeColumnsToContents(true);
      table->resize(1000, 250);
      table->setHighlightMouseOver(true);
      table->enableColumnHiding(0, true);
      table->enableColumnHiding(1, true);

//      table->setLayout(new Wt::WFitLayout());

    }
    Wt::WWidget * home(){
      return new Wt::WText("Files Home");
    }
};
}}}

