#include "Connection.h"
#include "Statement.h"
#include "tntdb/statement.h"
#include "ResultSet.h"
//#include <sqlite3.h>

using namespace org::esb::sql;


Statement::Statement(Connection & db, const char * sql):_con(db),_sql(sql){
//	cout << (char*)_con.mysql<<endl;

//	_stmt=db.prepare(sql);
}
Statement::~Statement(){
	mysql_free_result(res);
}
/*
Statement::Statement(tntdb::Statement & st){
	_stmt=st;
}
*/
//Statement::Statement(const Statement & st){}

//ResultSet & Statement::executeQuery(string sql, void * callback){}

ResultSet Statement::executeQuery(){
	mysql_query(_con.mysql, _sql);
	res=mysql_store_result(_con.mysql);
//	MYSQL_ROW _record=mysql_fetch_row(res);
//	cout << _record[0]<<endl;

	return ResultSet(res);
}

void Statement::execute(){


}

void Statement::bind(int pos, string ref){


}
void Statement::bind(int pos, int ref){


}
void Statement::bind(int pos, const void * ref, int size){


}

void Statement::close(){}

