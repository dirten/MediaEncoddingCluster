#ifndef CREATE_DATABASE
#define CREATE_DATABASE
#include "org/esb/io/File.h"
//#include <sqlite3.h>
#include <iostream>
#include "org/esb/sql/Connection.h"
#include "org/esb/sql/Statement.h"
#include "org/esb/sql/ResultSet.h"
//#include "tntdb/connect.h"

using namespace std;
using namespace org::esb::sql;
//using namespace tntdb;
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
bool checkDatabase(Connection & con){
    Statement stmt=con.createStatement("select * from version where component='database.model'");
  	ResultSet rs=stmt.executeQuery();
    if(rs.next()){
      if(rs.getString(2)==string("0.1.1"))return true;
    }
	return false;
}

void createTable(Connection & con,const char * sql){
	try{
  	    Statement stmt=con.createStatement(sql);
  	    stmt.execute();
	}catch(exception&ex){
	    cout << "Fehler:"<<ex.what()<<endl;
	}
}

bool createDatabase(Connection & con){
	createTable(con,"create table config (id integer(11) primary key auto_increment, configkey varchar(255) not null , configval varchar(255))");
	createTable(con,"create table files (id integer(11) primary key auto_increment ,filename varchar(255), size double, stream_count integer(2), title varchar(255), author varchar(255), copyright varchar(255), comment varchar(255), album varchar(255), year integer(4), track varchar(255), genre varchar(255), duration integer(11), bitrate integer(11), insertdate timestamp, type integer(1))");
	createTable(con,"create table streams(id integer(11) primary key auto_increment,fileid integer(11), stream_index integer(11), stream_type integer(11), codec integer(11), codec_name varchar(255) ,framerate integer(11), start_time integer(11), duration double, time_base_num integer(11),time_base_den integer(11), framecount integer(11), width integer(11), height integer(11), gop_size integer(11), pix_fmt integer(11), bit_rate integer(11), rate_emu integer(11), sample_rate integer(11), channels integer(11), sample_fmt integer(11), priv_data_size integer(11), priv_data mediumblob)");
	createTable(con,"create table packets (id integer(11) primary key auto_increment,stream_id integer(11),pts double,dts double,stream_index integer(11),key_frame integer(11), frame_group integer(11),flags integer(11),duration integer(11),pos integer(11),data_size integer(11),data mediumblob)");
	createTable(con,"create table jobs (id integer(11) primary key auto_increment, inputfile integer(11), outputfile integer(11), begin timestamp, complete timestamp)");
	createTable(con,"create table job_details (id integer(11) primary key auto_increment, job_id integer(11), instream integer(11), outstream integer(11))");
	createTable(con,"create table job_logs (id integer(11) primary key auto_increment, packet_in integer(11), packet_out integer(11), begin timestamp, complete timestamp)");
	createTable(con,"create table frame_groups(id integer(11) primary key auto_increment, frame_group integer(11), stream_id integer(11), complete timestamp)");
	createTable(con,"create table version (id integer(11) primary key auto_increment, component varchar(255), version varchar(255))");
	createTable(con,"create table profiles(id integer(11) primary key auto_increment, profile_name varchar(255), v_format integer(11), v_codec integer(11), v_bitrate integer(11),v_framerate integer(11),v_width integer(11), v_height integer(11), a_channels integer(11), a_codec integer(11), a_bitrate integer(11),a_samplerate integer(11))");
	createTable(con,"CREATE INDEX packet_group_idx on packets(frame_group)");
	createTable(con,"CREATE INDEX packet_pts_idx on packets(pts)");
	createTable(con,"insert into version (component, version) values ('database.model','0.0.1')");
	con.close();
	return true;
}

#endif

