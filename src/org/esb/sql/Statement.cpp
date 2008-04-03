#include "Connection.h"
#include "Statement.h"
#include "tntdb/statement.h"
#include "ResultSet.h"
//#include <sqlite3.h>

using namespace org::esb::sql;


Statement::Statement(Connection & db, const char * sql)/*:sqlite3_command((sqlite3_connection&)db, sql)*/{
//	_stmt=db.prepare(sql);
}

Statement::Statement(tntdb::Statement  st){
	_stmt=st;
}
//Statement::Statement(const Statement & st){}

//ResultSet & Statement::executeQuery(string sql, void * callback){}

ResultSet Statement::executeQuery(){return ResultSet(_stmt.select());}

void Statement::execute(){


}

void Statement::bind(int pos, string ref){


}
void Statement::bind(int pos, int ref){


}
void Statement::bind(int pos, const void * ref, int size){


}

void Statement::close(){}

