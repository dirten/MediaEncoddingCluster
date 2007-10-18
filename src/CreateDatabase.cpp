#include "org/esb/io/File.h"
#include <sqlite3.h>
using namespace std;
using namespace org::esb::io;

sqlite3 * getDatabase(File &databaseFile){
  sqlite3 *db;
    char *zErrMsg = 0;
    int rc = sqlite3_open(databaseFile.getPath(), &db);
    if( rc ){
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
		exit(1);
    }
  return db;
}

bool checkDatabase(File &databaseFile){


}

bool createDatabase(File &databaseFile){
	cout << "Creating Database"<< endl;
    sqlite3 *db =getDatabase(databaseFile);
    
	sqlite3_exec(db,"create table files (id,filename)",NULL,NULL,NULL);
	sqlite3_exec(db,"create table streams(id,fileid, stream_index, codec,framerate, start_time, duration, framecount)",NULL,NULL,NULL);
	sqlite3_exec(db,"create table packets (id,pts,dts,stream_index,flags,duration,pos,data_size,data)",NULL,NULL,NULL);
	sqlite3_close(db);
}
