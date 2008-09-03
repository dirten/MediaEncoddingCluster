#include <map>
#include <string>
#include <iostream>
#include <mysql/mysql.h>

#include "Column.h"
#include "ResultSetMetaData.h"
#include "SqlException.h"


namespace org{
namespace esb{
namespace sql{
class Row{
  public:
    Row(MYSQL_STMT * stmt):st(stmt){
//      std::cerr<<"Create Row Object"<<std::endl;
      int column_count=mysql_stmt_field_count(stmt);
      bind=new MYSQL_BIND[column_count];
      memset(bind,0,sizeof(MYSQL_BIND[column_count]));
      rsmd=new ResultSetMetaData(stmt);
      int count=rsmd->getColumnCount();
//      meta=mysql_stmt_result_metadata(stmt);

      for(int a=0;a<count;a++){
        Column *col=new Column(rsmd->getColumn(a), bind[a]);
        cols[std::string(rsmd->getColumn(a)->name)]=col;
        idx2name[a]=rsmd->getColumn(a)->name;
      }
      if (mysql_stmt_bind_result(stmt, bind)){
    	throw SqlException( std::string("failed while bind the result: ").append(mysql_stmt_error(stmt)));

//        throw SqlException( mysql_stmt_error(stmt));
      }
    }

    Column * getColumn(std::string name){
      Column * col=cols[name];
      if(col==NULL)throw SqlException(std::string("Column with name:").append(name).append(" not found") );
      return col;
    }

    Column * getColumn(int idx){
      return getColumn(idx2name[idx]);
    }
    ResultSetMetaData * getMetaData(){
      return rsmd;
    }
    bool next(){
      int res=mysql_stmt_fetch(st);
      if(res!=0&&res!=MYSQL_NO_DATA&&res!=MYSQL_DATA_TRUNCATED){
    		throw SqlException( std::string("failed while fetch the data: ").append(mysql_stmt_error(st)));

//          throw SqlException(mysql_stmt_error(st));
      }
      if(res==MYSQL_DATA_TRUNCATED){
      
      }
      
      return res!=MYSQL_NO_DATA;
    }

    ~Row(){
//      std::cerr<<"delete Row Object"<<std::endl;
      delete rsmd;
//      mysql_free_result(meta);
      std::map<std::string, Column*>::iterator it=cols.begin();
      for(;it!=cols.end();it++){
//        std::cerr << "Delete Column:"<<(*it).second->getName()<<std::endl;
        delete (*it).second;
      }
      delete []bind;
      cols.clear();
    }

  private:
    MYSQL_STMT * st;
    MYSQL_BIND * bind;
    MYSQL_RES * meta;
    ResultSetMetaData * rsmd;
    std::map<std::string, Column*> cols;
    std::map<int,std::string> idx2name;
};
}}}


