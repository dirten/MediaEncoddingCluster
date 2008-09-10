#include "org/esb/util/Decimal.h"

using namespace org::esb::util;
namespace org{
namespace esb{
namespace sql{
class Parameter{
  public:
    Parameter(MYSQL_STMT * stmt){
      const int column_count=mysql_stmt_param_count(stmt);
      bind=new MYSQL_BIND[column_count];
	  const int memsize=sizeof(MYSQL_BIND)*column_count;
      memset(bind,0,memsize);

      for(int a=0;a<column_count;a++){
        Column * col=new Column(bind[a]);
        cols[a]=col;
      }
    /*      
      if (mysql_stmt_bind_param(stmt, bind)){
        throw SqlException( mysql_stmt_error(stmt));
      } 
    */
    }

    Column * getParameter(int idx){
      Column * col=cols[idx];
      if(col==NULL)throw SqlException(std::string("Column with name:").append(Decimal(idx).toString()).append(" not found") );
      return col;
//      return cols[idx];
    }

    ~Parameter(){
      delete []bind;
      std::map<int, Column*>::iterator it=cols.begin();
      for(;it!=cols.end();it++){
        delete (*it).second;
      }
      cols.clear();
    }
//  private:
    MYSQL_STMT * st;
    MYSQL_BIND * bind;
    std::map<int, Column*> cols;

};
}}}

