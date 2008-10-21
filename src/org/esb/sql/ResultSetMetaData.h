#ifndef ORG_ESB_SQL_RESULTSETMETADATA_H
#define ORG_ESB_SQL_RESULTSETMETADATA_H
#include "my_sql.h"
#include <string>
namespace org{
namespace esb{
namespace sql{
class ResultSetMetaData{
  public:
    int getColumnCount(void);
    std::string getColumnName(int index);
    MYSQL_FIELD * getColumn(int index);
  private:
    friend class Row;
    ResultSetMetaData(MYSQL_STMT * stmt);
    ~ResultSetMetaData();
    MYSQL_RES * meta;
};
}}}
#endif
