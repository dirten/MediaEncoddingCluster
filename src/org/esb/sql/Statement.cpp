#include "Statement.h"
#include "ResultSet.h"
//#include <sqlite3.h>

using namespace org::esb::sql;


Statement::Statement(Connection & db, const char * sql):sqlite3_command((sqlite3_connection&)db, sql){}

ResultSet & Statement::executeQuery(string sql, void * callback){}

ResultSet & Statement::executeQuery(){return *new ResultSet(*this);}

void Statement::execute(string sql){}

void Statement::close(){}
