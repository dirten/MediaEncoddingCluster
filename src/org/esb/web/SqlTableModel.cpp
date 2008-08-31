#include "SqlTableModel.h"
#include "org/esb/sql/Connection.h"
#include "org/esb/sql/Statement.h"
#include "org/esb/sql/ResultSet.h"
#include "org/esb/sql/ResultSetMetaData.h"
#include <Wt/WString>
using namespace org::esb;
namespace org{
namespace esb{
namespace web{
    SqlTableModel::SqlTableModel(sql::ResultSet result,Wt::WObject* parent):Wt::WStandardItemModel(0,0, parent){
      sql::ResultSetMetaData * rsmd=result.getResultSetMetaData();
      int fieldCount=rsmd->getColumnCount();
      insertColumns(0,fieldCount);
      for(int a=0;a<fieldCount;a++){
      /*
      * for locale support 
      * if(!setHeaderData(a,Wt::WString::tr(rsmd->getColumnName(a).c_str()))){ 
      */
        if(!setHeaderData(a,rsmd->getColumnName(a))){
          std::cout<<"Failed"<<std::endl;
        }
      }
      for(int a=0;result.next();a++){
        if(rowCount()<=a)
          insertRow(rowCount());
        for(int b=0;b<fieldCount;b++)
          if(!setData(a,b,result.getString(b))){
            std::cout<<"set data Failed"<<std::endl;
          }
      }      
    }
}}}

