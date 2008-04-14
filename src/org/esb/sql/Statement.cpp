#include "Connection.h"
#include "Statement.h"
#include "ResultSet.h"
//#include <sqlite3.h>

using namespace org::esb::sql;

Statement::Statement(tntdb::Statement  stmt){
	tntstmt=stmt;
	res=0;
}

Statement::~Statement(){
	close();
}

ResultSet Statement::executeQuery(){
	return ResultSet(tntstmt);
}

ResultSet Statement::executeQuery(char* tmp){

}

bool Statement::execute(){
	return tntstmt.execute()==0;
}

void Statement::close(){
	tntstmt.clear();
}

