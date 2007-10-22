#include "org/esb/io/File.h"
#include <sqlite3.h>
#include <iostream>

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

	return false;
}

bool createDatabase(File &databaseFile){
	cout << "Creating Database"<< endl;
    char *zErrMsg = 0;
	int rc=0;
    sqlite3 *db =getDatabase(databaseFile);
    
	rc = sqlite3_exec(db,"create table files (id integer  primary key autoincrement ,filename)",NULL,NULL,&zErrMsg);
	if(rc!=SQLITE_OK)
		cout << zErrMsg<<endl;;
	rc=sqlite3_exec(db,"create table streams(id integer  primary key autoincrement,fileid, stream_index, stream_type, codec,framerate, start_time, duration, time_base, framecount, width, height, gop_size, pix_fmt, rate_emu, sample_rate, channels, sample_fmt)",NULL,NULL,&zErrMsg);
	if(rc!=SQLITE_OK)
		cout << zErrMsg<<endl;;
	rc=sqlite3_exec(db,"create table packets (id integer  primary key autoincrement,stream_id,pts,dts,stream_index,key_frame, frame_group,flags,duration,pos,data_size,data)",NULL,NULL,&zErrMsg);
	if(rc!=SQLITE_OK)
		cout << zErrMsg<<endl;;
	rc=sqlite3_exec(db,"create table jobs (id integer  primary key autoincrement, infile integer, outfile integer)",NULL,NULL,&zErrMsg);
	if(rc!=SQLITE_OK)
		cout << zErrMsg<<endl;;
	rc=sqlite3_exec(db,"create table job_details (id integer  primary key autoincrement, job_id integer, instream integer, outstream integer)",NULL,NULL,&zErrMsg);
	if(rc!=SQLITE_OK)
		cout << zErrMsg<<endl;;
	rc=sqlite3_exec(db,"create table job_logs (id integer  primary key autoincrement, begin, complete)",NULL,NULL,&zErrMsg);
	if(rc!=SQLITE_OK)
		cout << zErrMsg<<endl;;
	rc=sqlite3_exec(db,"create table version (id integer  primary key autoincrement, component, version)",NULL,NULL,&zErrMsg);
	if(rc!=SQLITE_OK)
		cout << zErrMsg<<endl;;
	rc=sqlite3_exec(db,"insert into version (component, version) values ('database.model','0.1.1')",NULL,NULL,&zErrMsg);
	if(rc!=SQLITE_OK)
		cout << zErrMsg<<endl;;
	sqlite3_close(db);
}
