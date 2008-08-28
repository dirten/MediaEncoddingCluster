#include "Connection.h"
#include "Statement.h"
#include "ResultSet.h"

using namespace org::esb::sql;

Statement::Statement(MYSQL & mysql, const char * s){
  stmt = mysql_stmt_init(&mysql);
  if (!stmt){
    throw SqlException( "mysql_stmt_init(), out of memory");
  }
  sql=s;
  if (mysql_stmt_prepare(stmt, sql.c_str(), strlen(sql.c_str()))){
    throw SqlException( mysql_stmt_error(stmt));
  }
}

Statement::~Statement(){
	close();
}

ResultSet Statement::executeQuery(){
    execute();
	return ResultSet(*stmt);
}

ResultSet Statement::executeQuery(char* tmp){
//  return NULL;
}

bool Statement::execute(){
  if (mysql_stmt_execute(stmt)){
    throw SqlException( mysql_stmt_error(stmt));
  }
  return true;
}

void Statement::close(){
  if (mysql_stmt_close(stmt)){
    throw SqlException( string("failed while closing the statement: ").append( mysql_stmt_error(stmt)));
  }
}

