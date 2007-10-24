#include "Statement.h"
#include <sqlite3.h>

using namespace org::esb::sql;


Statement::Statement(sqlite3 * db){
	_db=db;

}

ResultSet & Statement::executeQuery(string sql, void * callback){
	char *zErrMsg;
	int rc = sqlite3_exec(_db, sql.c_str(), (int (*)(void*, int, char**, char**))callback, 0, &zErrMsg);
}

void Statement::execute(string sql){
	char *zErrMsg;
	int rc = sqlite3_exec(_db, sql.c_str(), NULL, 0, &zErrMsg);
}

void Statement::close(){
}
