#ifndef CREATE_DATABASE
#define CREATE_DATABASE
#include "org/esb/io/File.h"
//#include <sqlite3.h>
#include <iostream>
#include "org/esb/sql/Connection.h"

using namespace std;
using namespace org::esb::io;
using namespace org::esb::sql;
//using namespace sqlite;
/*
sqlite3 *db=NULL;
sqlite3 * getDatabase(File &databaseFile){
    char *zErrMsg = 0;
    if(db==NULL){
	printf("Opening Database Connection\n");
    int rc = sqlite3_open(databaseFile.getPath(), &db);
    if( rc ){
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
		exit(1);
    }
    }
  return db;
}
*/
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
	createTable(con,"create table files (id integer  primary key autoincrement ,filename, size, stream_count, title, author, copyright, comment, album, year, track, genre, duration, bitrate, insertdate,type)");
	createTable(con,"create table streams(id integer  primary key autoincrement,fileid, stream_index, stream_type, codec, codec_name,framerate, start_time, duration, time_base_num,time_base_den, framecount, width, height, gop_size, pix_fmt, bit_rate, rate_emu, sample_rate, channels, sample_fmt)");
	createTable(con,"create table packets (id integer  primary key autoincrement,stream_id,pts,dts,stream_index,key_frame, frame_group,flags,duration,pos,data_size,data)");
	createTable(con,"create table jobs (id integer  primary key autoincrement, infile integer, outfile integer, begin date, complete date)");
	createTable(con,"create table job_details (id integer  primary key autoincrement, job_id integer, instream integer, outstream integer)");
	createTable(con,"create table job_logs (id integer  primary key autoincrement, packet_in integer,packet_out integer, begin date, complete date)");
	createTable(con,"create table frame_groups(id integer primary key autoincrement, frame_group integer, stream_id integer, complete timestamp)");
	createTable(con,"create table version (id integer  primary key autoincrement, component, version)");
	createTable(con,"insert into version (component, version) values ('database.model','0.1.1')");
	createTable(con,"create table profiles(id integer primary key autoincrement, profile_name, v_format, v_codec, v_bitrate,v_framerate,v_width, v_height, a_channels, a_codec, a_bitrate,a_samplerate)");
	createTable(con,"CREATE INDEX packet_group_idx on packets(frame_group)");
	createTable(con,"CREATE INDEX packet_pts_idx on packets(pts)");
	con.close();
	return true;
}

#endif

