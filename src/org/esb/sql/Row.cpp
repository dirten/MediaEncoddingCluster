#include <map>
#include <string>
#include <mysql/mysql.h>

#include "Column.h"
#include "SqlException.h"

namespace org{
namespace esb{
namespace sql{
class Row{
  public:
    Row(MYSQL_STMT * stmt):st(stmt){
      int column_count=mysql_stmt_field_count(stmt);
      bind=new MYSQL_BIND[column_count];
      memset(bind,0,sizeof(MYSQL_BIND[column_count]));
      meta=mysql_stmt_result_metadata(stmt);

      for(int a=0;MYSQL_FIELD * field=mysql_fetch_field(meta);a++){
        Column *col=new Column(field, bind[a]);
        cols[std::string(field->name)]=col;
        idx2name[a]=field->name;
      }
      if (mysql_stmt_bind_result(stmt, bind)){
        throw SqlException( mysql_stmt_error(stmt));
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

    bool next(){
      int res=mysql_stmt_fetch(st);
      if(res!=0&&res!=MYSQL_NO_DATA&&res!=MYSQL_DATA_TRUNCATED){
          throw SqlException(mysql_stmt_error(st));
      }
      if(res==MYSQL_DATA_TRUNCATED){
      
      }
      
      return res!=MYSQL_NO_DATA;
    }

    ~Row(){
      delete []bind;
      mysql_free_result(meta);
      std::map<std::string, Column*>::iterator it=cols.begin();
      for(;it!=cols.end();it++){
        delete (*it).second;
      }
      cols.clear();
    }

  private:
    MYSQL_STMT * st;
    MYSQL_BIND * bind;
    MYSQL_RES * meta;
    std::map<std::string, Column*> cols;
    std::map<int,std::string> idx2name;
};
}}}


