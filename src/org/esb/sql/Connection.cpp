#include "org/esb/io/File.h"
#include "Connection.h"
#include "Statement.h"
#include <sqlite3.h>
using namespace org::esb::sql;
using namespace org::esb::io;


Connection::Connection(File & databaseFile){
	printf("Opening Database Connection\n");
    int rc = sqlite3_open(databaseFile.getPath(), &_db);
    if( rc ){
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(_db));
        sqlite3_close(_db);
		exit(1);
    }
}

Statement & Connection::createStatement(){
	Statement *stmt=new Statement(_db);
	return *stmt;
}

void Connection::close(){
        sqlite3_close(_db);	
}
