/*

*/
#include "org/esb/io/File.h"
#include "CreateDatabase.cpp"


int main(int argc, char*argv[]){
	File databaseFile(argv[1]);
	sqlite3 * db=getDatabase(databaseFile);

	string sql;
	char*zErrMsg=0;
	int rc=0;
	int field=1;
	sql="insert into files (filename)values('../output.avi')";
	rc = sqlite3_exec(db,sql.c_str(),NULL,NULL,&zErrMsg);
	if(rc!=SQLITE_OK)
		cout << zErrMsg<<endl;;
    int fileid =sqlite3_last_insert_rowid(db);



    sqlite3_stmt *pStmt;
    sql="insert into jobs (infile, outfile)values(1,?)";
    sqlite3_prepare( db, sql.c_str(), sql.size(), &pStmt,  NULL );
    sqlite3_bind_int( pStmt, 1, fileid);
    rc=sqlite3_step(pStmt);
    rc = sqlite3_reset(pStmt);
    int jobid =sqlite3_last_insert_rowid(db);




	field=1;
	sql="insert into streams(fileid, stream_index, stream_type, codec, framerate, start_time, duration, time_base_num,time_base_den, framecount, width, height, gop_size, pix_fmt, bit_rate, rate_emu, sample_rate, channels, sample_fmt)values(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)";
    sqlite3_prepare( db, sql.c_str(), sql.size(), &pStmt,  NULL );
    sqlite3_bind_int( pStmt, field++, fileid);
    sqlite3_bind_int( pStmt, field++, 1);
    sqlite3_bind_int( pStmt, field++, 1);
    sqlite3_bind_int( pStmt, field++, 86017);
    sqlite3_bind_int( pStmt, field++, 0);
    sqlite3_bind_int( pStmt, field++, 0);
    sqlite3_bind_int( pStmt, field++, 99573495);
    sqlite3_bind_int( pStmt, field++, 1);
    sqlite3_bind_int( pStmt, field++, 15963);
    sqlite3_bind_int( pStmt, field++, 0);
    sqlite3_bind_int( pStmt, field++, 0);
    sqlite3_bind_int( pStmt, field++, 0);
    sqlite3_bind_int( pStmt, field++, 12);
    sqlite3_bind_int( pStmt, field++, 0);
    sqlite3_bind_int( pStmt, field++, 128000);
    sqlite3_bind_int( pStmt, field++, 0);
    sqlite3_bind_int( pStmt, field++, 44100);
    sqlite3_bind_int( pStmt, field++, 2);
    sqlite3_bind_int( pStmt, field++, 1);
    rc=sqlite3_step(pStmt);
    rc = sqlite3_reset(pStmt);
    int streamid =sqlite3_last_insert_rowid(db);


	sql="insert into job_details (job_id,instream, outstream)values(?,?,?)";
    sqlite3_prepare( db, sql.c_str(), sql.size(), &pStmt,  NULL );
    sqlite3_bind_int( pStmt, 1, jobid);
    sqlite3_bind_int( pStmt, 2, 2);
    sqlite3_bind_int( pStmt, 3, streamid);
    rc=sqlite3_step(pStmt);
    rc = sqlite3_reset(pStmt);




	field=1;
	sql="insert into streams(fileid, stream_index, stream_type, codec, framerate, start_time, duration, time_base_num,time_base_den, framecount, width, height, gop_size, pix_fmt, bit_rate, rate_emu, sample_rate, channels, sample_fmt)values(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)";
    
    sqlite3_prepare( db, sql.c_str(), sql.size(), &pStmt,  NULL );
    sqlite3_bind_int( pStmt, field++, fileid);
    sqlite3_bind_int( pStmt, field++, 0);
    sqlite3_bind_int( pStmt, field++, 0);
    sqlite3_bind_int( pStmt, field++, 17);
    sqlite3_bind_int( pStmt, field++, 25);
    sqlite3_bind_int( pStmt, field++, 0);
    sqlite3_bind_int( pStmt, field++, 156007);
    sqlite3_bind_int( pStmt, field++, 1);
    sqlite3_bind_int( pStmt, field++, 25);
    sqlite3_bind_int( pStmt, field++, 0);
    sqlite3_bind_int( pStmt, field++, 512);
    sqlite3_bind_int( pStmt, field++, 256);
    sqlite3_bind_int( pStmt, field++, 100);
    sqlite3_bind_int( pStmt, field++, 0);
    sqlite3_bind_int( pStmt, field++, 4000000);
    sqlite3_bind_int( pStmt, field++, 0);
    sqlite3_bind_int( pStmt, field++, 0);
    sqlite3_bind_int( pStmt, field++, 0);
    sqlite3_bind_int( pStmt, field++, 1);
    rc=sqlite3_step(pStmt);
    rc = sqlite3_reset(pStmt);
    streamid =sqlite3_last_insert_rowid(db);


    sql="insert into job_details (job_id,instream, outstream)values(?,?,?)";
    sqlite3_prepare( db, sql.c_str(), sql.size(), &pStmt,  NULL );
    sqlite3_bind_int( pStmt, 1, jobid);
    sqlite3_bind_int( pStmt, 2, 1);
    sqlite3_bind_int( pStmt, 3, streamid);
    rc=sqlite3_step(pStmt);
    rc = sqlite3_reset(pStmt);



}
