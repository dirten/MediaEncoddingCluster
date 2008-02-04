#include "org/esb/io/File.h"
#include "Connection.h"
#include "Statement.h"
//#include <sqlite3.h>
using namespace org::esb::sql;
using namespace org::esb::io;


Connection::Connection(char*filename):sqlite3_connection(filename){
    printf("Opening Database Connection\n");
//    open(filename);
}

Connection::Connection(File & databaseFile){
    printf("Opening Database Connection\n");
    open(databaseFile.getPath());
}

Statement & Connection::createStatement(const char * sql){
	Statement *stmt=new Statement(*this, sql);
	return *stmt;
}
PreparedStatement & Connection::prepareStatement(const char * sql){
	PreparedStatement *stmt=new PreparedStatement(*this, sql);
	return *stmt;
}

Statement & Connection::createStatement(){}

void Connection::close(){
//        sqlite3_close(_db);	
}
