#include "ResultSetMetaData.h"
namespace org{
namespace esb{
namespace sql{
ResultSetMetaData::ResultSetMetaData(MYSQL_STMT * stmt){
  meta=mysql_stmt_result_metadata(stmt);
}

int ResultSetMetaData::getColumnCount(){
  return mysql_num_fields(meta);
}
std::string ResultSetMetaData::getColumnName(int index){
  mysql_field_seek(meta, index);
  return mysql_fetch_field(meta)->name;
}
}}}
