#include "org/esb/io/File.h"
#include <sqlite3.h>
#include <iostream>
#include "org/esb/sql/Connection.h"

using namespace std;
using namespace org::esb::io;
using namespace org::esb::sql;
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

void createTable(Connection & con,const char * sql){
	try{
    	    con.executenonquery(sql);
	}catch(exception&ex){
	    cout << "Fehler:"<<ex.what()<<endl;
	}

}

bool createDatabase(File &databaseFile){
	Connection con(databaseFile);
	createTable(con,"create table files (id integer  primary key autoincrement ,filename)");
	createTable(con,"create table streams(id integer  primary key autoincrement,fileid, stream_index, stream_type, codec,framerate, start_time, duration, time_base, framecount, width, height, gop_size, pix_fmt, rate_emu, sample_rate, channels, sample_fmt)");
	createTable(con,"create table packets (id integer  primary key autoincrement,stream_id,pts,dts,stream_index,key_frame, frame_group,flags,duration,pos,data_size,data)");
	createTable(con,"create table jobs (id integer  primary key autoincrement, infile integer, outfile integer, begin date, complete date)");
	createTable(con,"create table job_details (id integer  primary key autoincrement, job_id integer, instream integer, outstream integer)");
	createTable(con,"create table job_logs (id integer  primary key autoincrement, packet_in integer,packet_out integer, begin date, complete date)");
	createTable(con,"create table version (id integer  primary key autoincrement, component, version)");
	createTable(con,"insert into version (component, version) values ('database.model','0.1.1')");
	con.close();
}
