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
	if(res!=NULL)
	    delete res;
}

ResultSet Statement::executeQuery(){
	return ResultSet(tntstmt);
}

ResultSet & Statement::executeQuery(char* tmp){
	if(res!=NULL)
	    delete res;
	res=0;
	res=new ResultSet(tntstmt);
//	ResultSet rs(res);
	return *res;
}

bool Statement::execute(){
	return tntstmt.execute()==0;
}

void Statement::close(){


}

