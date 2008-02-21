#include "org/esb/io/File.h"
#include "Connection.h"
#include "Statement.h"
//#include <sqlite3.h>
#include <iostream>
using namespace org::esb::sql;
using namespace org::esb::io;
//void __attribute__ ((constructor)) my_init(void);
//void _init() __attribute__((constructor));

Connection::Connection(char*filename):sqlite3_connection(filename){
//    printf("Opening Database Connection\n");
}

sqlite3_transaction Connection::getTransaction(){
    return sqlite3_transaction(*this);
}

Connection::Connection(File & databaseFile):sqlite3_connection(databaseFile.getPath()){
//    printf("Opening Database Connection\n");
}

Statement Connection::createStatement(const char * sql){
//	_tmpStatement=Statement(*this, sql);
	return Statement(*this, sql);
}
PreparedStatement & Connection::prepareStatement(const char * sql){
	PreparedStatement *stmt=new PreparedStatement(*this, sql);
	return *stmt;
}

Statement & Connection::createStatement(){}


void Connection::close(){
    sqlite3_connection::close();	
}
/*
void
//_init(int argc, char *argv[], char *envp[])
my_init()
{
    std::cout << "Lib SQL Init"<<std::endl;
}
*/
