
#include <sqlite3.h>
#include <iostream>
#include "org/esb/io/File.h"
#include "org/esb/av/FormatInputStream.h"
#include "org/esb/av/PacketInputStream.h"
#include "org/esb/av/Packet.h"
#include "org/esb/av/Codec.h"
#include "CreateDatabase.cpp"
#include <avformat.h>
using namespace std;
using namespace org::esb::io;
using namespace org::esb::av;

int main(int argc, char * argv[]){
	cout << LIBAVCODEC_IDENT <<endl;
	if(argc!=3){
		cout << "wrong parameter count"<<endl;	
		exit(1);
	}

	File databaseFile(argv[1]);
	if(!databaseFile.exists()||!checkDatabase(databaseFile)){
		createDatabase(databaseFile);
	}

	File inputFile(argv[2]);
	if(!inputFile.canRead()){
		cout << "Source File not found"<<endl;
	}

    sqlite3_stmt *pStmt;
    char *zErrMsg = 0;
	sqlite3 * db=getDatabase(databaseFile);
//	sqlite3 * db=getDatabase(databaseFile);

    string sql="insert into packets(id,pts,dts,stream_index,key_frame, frame_group,flags,duration,pos,data_size,data) values (NULL,?,?,?,?,?,?,?,?,?,?)";

    sqlite3_prepare( db, sql.c_str(), sql.size(), &pStmt,  NULL );

	string sqlFile="INSERT INTO files(filename) values ( '"; 
		sqlFile+=inputFile.getPath();
		sqlFile+="')";
	
    sqlite3_exec(db,sqlFile.c_str(),NULL,NULL,NULL);
    int fileid =sqlite3_last_insert_rowid(db);


    FormatInputStream fis(&inputFile);
    AVFormatContext * ctx=fis.getFormatContext();

	for(int a =0;a<ctx->nb_streams;a++){
		string sqlStreams="insert into streams (fileid,stream_index,codec,framerate,start_time,duration) values(";
		char  values[1000];
		sprintf(values,"%d,%d,%d,%d,%d,%d",fileid,a,ctx->streams[a]->codec->codec_id,ctx->streams[a]->r_frame_rate,ctx->streams[a]->start_time,ctx->streams[a]->duration);
		sqlStreams+=values;
		sqlStreams+=")";
		char *zErrMsg = 0;
	    sqlite3_exec(db,sqlStreams.c_str(),NULL,NULL,&zErrMsg);
    	

    }
    sqlite3_exec(db,"BEGIN TRANSACTION",NULL,NULL,NULL);


	int streamCount=fis.getStreamCount();
	cout << "StreamCount="<<streamCount<<endl;
    PacketInputStream pis(&fis);
    Packet packet;


    int count=0, frame_group=0;
    while(true&&count < 20000){
        packet=pis.readPacket();
        if(packet.data==NULL)break;
	if(++count%1000==0)cout << count << "Packets in db"<<endl;
	if(packet.stream_index==0&&packet.isKeyFrame())frame_group++;

        sqlite3_bind_int( pStmt, 1, packet.pts);
        sqlite3_bind_int( pStmt, 2, packet.dts);
        sqlite3_bind_int( pStmt, 3, packet.stream_index);
        sqlite3_bind_int( pStmt, 4, packet.isKeyFrame());
	if(packet.stream_index==0)
    	    sqlite3_bind_int( pStmt, 5, frame_group);
	else
    	    sqlite3_bind_null( pStmt, 5);	
        sqlite3_bind_int( pStmt, 6, packet.flags);
        sqlite3_bind_int( pStmt, 7, packet.duration);
        sqlite3_bind_int( pStmt, 8, packet.pos);
        sqlite3_bind_int( pStmt, 9, packet.size);
        sqlite3_bind_blob( pStmt, 10, (char*)packet.data,packet.size, SQLITE_STATIC );
        int rc=sqlite3_step(pStmt);

        rc = sqlite3_reset(pStmt);
		if( rc!=SQLITE_OK ){
           fprintf(stderr, "SQL error: %s\n", zErrMsg);
	   		sqlite3_free(zErrMsg);
        }
    }
    sqlite3_exec(db,"commit",NULL,NULL,NULL);

    sqlite3_close(db);


	return 0;
}
