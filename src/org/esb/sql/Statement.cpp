#include "Statement.h"
#include "ResultSet.h"
//#include <sqlite3.h>

using namespace org::esb::sql;


Statement::Statement(Connection & db, const char * sql):sqlite3_command((sqlite3_connection&)db, sql){}

Statement::Statement(const Statement & st):sqlite3_command((sqlite3_command&)st){}

ResultSet & Statement::executeQuery(string sql, void * callback){}

ResultSet Statement::executeQuery(){return ResultSet(*this);}

void Statement::execute(){executenonquery();}

void Statement::close(){}

