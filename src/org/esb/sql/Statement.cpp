#include "Connection.h"
#include "Statement.h"
#include "ResultSet.h"
//#include <sqlite3.h>

using namespace org::esb::sql;

Statement::Statement(tntdb::Statement  stmt){
	tntstmt=stmt;
}
Statement::~Statement(){

}

ResultSet Statement::executeQuery(){
	return ResultSet(tntstmt.select());
}

void Statement::execute(){
	tntstmt.execute();
}

void Statement::close(){


}

