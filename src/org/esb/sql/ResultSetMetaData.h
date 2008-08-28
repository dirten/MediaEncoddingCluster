#ifndef ORG_ESB_SQL_RESULTSETMETADATA_H
#define ORG_ESB_SQL_RESULTSETMETADATA_H

#include <mysql/mysql.h>
#include <string>
namespace org{
namespace esb{
namespace sql{
class ResultSetMetaData{
  public:
    int getColumnCount(void);
    std::string getColumnName(int index);
  private:
    friend class Row;
    ResultSetMetaData(MYSQL_STMT * stmt);
//    MYSQL_RES * meta;
    MYSQL_RES * meta;


};
}}}
#endif
