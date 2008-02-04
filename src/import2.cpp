
#include <sqlite3.h>
#include <iostream>
#include <fstream>
#include "org/esb/io/File.h"
#include "org/esb/av/FormatInputStream.h"
#include "org/esb/av/PacketInputStream.h"
#include "org/esb/av/Packet.h"
#include "org/esb/av/Codec.h"
#include "CreateDatabase.cpp"
#include <avformat.h>
#include <boost/progress.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/polymorphic_binary_iarchive.hpp> 
#include <boost/archive/polymorphic_binary_oarchive.hpp> 

using namespace std;
using namespace org::esb::io;
using namespace org::esb::av;
using namespace boost;



int main(int argc, char * argv[]){
	cout << LIBAVCODEC_IDENT <<endl;
	if(argc!=3){
		cout << "wrong parameter count"<<endl;	
		exit(1);
	}

	File databaseFile(argv[1]);
	if(!databaseFile.exists()||!checkDatabase(databaseFile)){
		createDatabase(databaseFile);
	}else{
	    cout << "Database not found";
	}

	File inputFile(argv[2]);
	if(!inputFile.canRead()){
		cout << "Source File not found"<<endl;
	}

    sqlite3_stmt *pStmt,*pStmt2;
    char *zErrMsg = 0;
	sqlite3 * db=getDatabase(databaseFile);

    string sql="insert into packets(id,stream_id,pts,dts,stream_index,key_frame, frame_group,flags,duration,pos,data_size,data) values (NULL,?,?,?,?,?,?,?,?,?,?,?)";


	string sqlFile="INSERT INTO files(filename) values ( '"; 
		sqlFile+=inputFile.getPath();
		sqlFile+="')";
	
    sqlite3_exec(db,sqlFile.c_str(),NULL,NULL,NULL);
    int fileid =sqlite3_last_insert_rowid(db);


    FormatInputStream fis(&inputFile);
    AVFormatContext * ctx=fis.getFormatContext();
	int streams[10];
	string sqlStreams="insert into streams (fileid,stream_index, stream_type,codec,framerate,start_time,duration,time_base, width, height, gop_size, pix_fmt, rate_emu, sample_rate, channels, sample_fmt) values (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)";
    sqlite3_prepare( db, sqlStreams.c_str(), sqlStreams.size(), &pStmt,  NULL );
//           fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
	for(int a =0;a<ctx->nb_streams;a++){
		char *zErrMsg = 0;
		int field=1;
	    sqlite3_bind_int( pStmt, field++, fileid);
	    sqlite3_bind_int( pStmt, field++, a);
	    sqlite3_bind_int( pStmt, field++, ctx->streams[a]->codec->codec_type);
	    sqlite3_bind_int( pStmt, field++, ctx->streams[a]->codec->codec_id);
	    sqlite3_bind_int( pStmt, field++, av_q2d(ctx->streams[a]->r_frame_rate));
	    sqlite3_bind_int( pStmt, field++, ctx->streams[a]->start_time);
	    sqlite3_bind_int( pStmt, field++, ctx->streams[a]->duration);
	    sqlite3_bind_int( pStmt, field++, av_q2d(ctx->streams[a]->time_base));
	    sqlite3_bind_int( pStmt, field++, ctx->streams[a]->codec->width);
	    sqlite3_bind_int( pStmt, field++, ctx->streams[a]->codec->height);
	    sqlite3_bind_int( pStmt, field++, ctx->streams[a]->codec->gop_size);
	    sqlite3_bind_int( pStmt, field++, ctx->streams[a]->codec->pix_fmt);
	    sqlite3_bind_int( pStmt, field++, ctx->streams[a]->codec->rate_emu);
	    sqlite3_bind_int( pStmt, field++, ctx->streams[a]->codec->sample_rate);
	    sqlite3_bind_int( pStmt, field++, ctx->streams[a]->codec->channels);
	    sqlite3_bind_int( pStmt, field++, ctx->streams[a]->codec->sample_fmt);
        int rc=sqlite3_step(pStmt);
        sqlite3_reset(pStmt);
//        sqlite3_clear_bindings(pStmt);

		if( rc!=SQLITE_OK ){
//           fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
        }
    	int streamid =sqlite3_last_insert_rowid(db);
    	
		cout << "LastInsertId"<<streamid<<endl;
    	streams[a]=streamid;

    }
    sqlite3_exec(db,"BEGIN TRANSACTION",NULL,NULL,NULL);


	int streamCount=fis.getStreamCount();
	cout << "StreamCount="<<streamCount<<endl;
    PacketInputStream pis(&fis);
//    const Packet  packet;
    sqlite3_prepare( db, sql.c_str(), sql.size(), &pStmt,  NULL );


    int count=0, frame_group=0;
//	ProgressBar pBar;
//	pBar.setMaximum(394000);
	progress_display show_progress(394000);

//    const test_serial ts(1,2,1);
    
//    bos << ts;

    while(true/*&&count < 1000*/){

        Packet packet;
        pis.readPacket(packet);
	/*
	char filename[100];
	sprintf(filename,"/tmp/hive/data.%d",count);
	ofstream ofs(filename,ios::binary);
	boost::archive::text_oarchive bos(ofs);
//	const Packet packet2;
	bos << (const Packet)packet;
	*/
        if(packet.data==NULL)break;
        
        ++count;
	++show_progress;
//	if(++count%1000==0){
//		cout << count << "Packets in db"<<endl;
	
//	}
//		pBar.setValue(count);

	if(packet.stream_index==0&&packet.isKeyFrame())frame_group++;
	int  field=1;
//		cout << "Stream:"<<streams[packet.stream_index]<<endl;
        sqlite3_bind_int( pStmt, field++, streams[packet.stream_index]);
        sqlite3_bind_int( pStmt, field++, packet.pts);
        sqlite3_bind_int( pStmt, field++, packet.dts);
        sqlite3_bind_int( pStmt, field++, packet.stream_index);
        sqlite3_bind_int( pStmt, field++, packet.isKeyFrame());
	if(packet.stream_index==0)
    	    sqlite3_bind_int( pStmt, field++, frame_group);
	else
    	    sqlite3_bind_null( pStmt, field++);	
        sqlite3_bind_int( pStmt, field++, packet.flags);
        sqlite3_bind_int( pStmt, field++, packet.duration);
        sqlite3_bind_int( pStmt, field++, packet.pos);
        sqlite3_bind_int( pStmt, field++, packet.size);
        sqlite3_bind_blob( pStmt, field++, (char*)packet.data,packet.size, SQLITE_STATIC );
        int rc=sqlite3_step(pStmt);

        rc = sqlite3_reset(pStmt);
		if( rc!=SQLITE_OK ){
           fprintf(stderr, "SQL error: %s\n", zErrMsg);
	   		sqlite3_free(zErrMsg);
        }
    }
    sqlite3_exec(db,"commit",NULL,NULL,NULL);

    sqlite3_close(db);
//    cout << endl;
//    cout.flush();
	return 0;
}
