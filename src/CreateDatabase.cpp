#include "org/esb/io/File.h"
#include <sqlite3.h>
using namespace std;
using namespace org::esb::io;
sqlite3 *db=NULL;

sqlite3 * getDatabase(File &databaseFile){
    char *zErrMsg = 0;
//    if(db==NULL){
	printf("Opening Database Connection\n");
    int rc = sqlite3_open(databaseFile.getPath(), &db);
    if( rc ){
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
		exit(1);
    }
//    }
  return db;
}

bool checkDatabase(File &databaseFile){


}

bool createDatabase(File &databaseFile){
	cout << "Creating Database"<< endl;
    char *zErrMsg = 0;

    sqlite3 *db =getDatabase(databaseFile);
    
	sqlite3_exec(db,"create table files (id integer  primary key autoincrement ,filename)",NULL,NULL,&zErrMsg);
	cout << zErrMsg;
	sqlite3_errmsg(db);
	sqlite3_exec(db,"create table streams(id ,fileid, stream_index, codec,framerate, start_time, duration, framecount)",NULL,NULL,&zErrMsg);
	cout << zErrMsg;
	sqlite3_errmsg(db);
	sqlite3_exec(db,"create table packets (id integer  primary key autoincrement,pts,dts,stream_index,key_frame, frame_group,flags,duration,pos,data_size,data)",NULL,NULL,&zErrMsg);
	cout << zErrMsg;
	sqlite3_errmsg(db);
	sqlite3_close(db);
}
