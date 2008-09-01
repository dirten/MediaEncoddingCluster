#include "ResultSetMetaData.h"
namespace org{
namespace esb{
namespace sql{
ResultSetMetaData::ResultSetMetaData(MYSQL_STMT * stmt){
  meta=mysql_stmt_result_metadata(stmt);
}
ResultSetMetaData::~ResultSetMetaData(){
  mysql_free_result(meta);
}

int ResultSetMetaData::getColumnCount(){
  return mysql_num_fields(meta);
}

MYSQL_FIELD * ResultSetMetaData::getColumn(int idx){
  mysql_field_seek(meta, idx);
  return mysql_fetch_field(meta);
}
std::string ResultSetMetaData::getColumnName(int index){
  return getColumn(index)->name;
}

}}}
