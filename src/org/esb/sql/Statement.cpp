#include "Connection.h"
#include "Statement.h"
#include "tntdb/statement.h"
#include "ResultSet.h"
//#include <sqlite3.h>

using namespace org::esb::sql;


Statement::Statement(Connection & db, const char * sql):_con(db),_sql(sql){
    _stmt=mysql_stmt_init(_con.mysql);
}
Statement::~Statement(){
//	mysql_free_result(_res);
}


//ResultSet & Statement::executeQuery(string sql, void * callback){}

ResultSet Statement::executeQuery(){
	if(mysql_stmt_prepare(_stmt,_sql, strlen(_sql))){
    	fprintf(stderr, " mysql_stmt_prepare(), SELECT failed\n");
    	fprintf(stderr, " %s\n", mysql_stmt_error(_stmt));
    	exit(0);
	}
    
	if (mysql_stmt_execute(_stmt)){
	    fprintf(stderr, " mysql_stmt_execute(), failed\n");
	    fprintf(stderr, " %s\n", mysql_stmt_error(_stmt));
	    exit(0);
	}
	return ResultSet(*this->_stmt);
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

