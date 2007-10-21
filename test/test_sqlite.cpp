#include "org/esb/io/File.h"
#include "org/esb/av/FormatInputStream.h"
#include "org/esb/av/PacketInputStream.h"
#include "org/esb/av/Packet.h"
#include <sqlite3.h>
#include <iostream>
#include <iomanip>

using namespace std;
using namespace org::esb::io;
using namespace org::esb::av;

int main(){

    sqlite3 *db;
    sqlite3_stmt *pStmt;
    char *zErrMsg = 0;
//    int rc = sqlite3_open("/tmp/hive/test.db", &db);
    int rc = sqlite3_open("/tmp/hive/hive.db", &db);
//    int rc = sqlite3_open("/media/jh/hive.db", &db);
    if( rc ){
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
	exit(1);
    }
//    string sql="insert into packet(id,intime,job,format,codec,width,height,packet_size,packet) values (NULL,NULL,1,?,?,?,?,?,?)";
    string sql="insert into packet(stream_id,pts,dts,flags,duration,position,packet_size,packet_data,key) values (?,?,?,?,?,?,?,?,1)";
    rc=sqlite3_prepare( db, sql.c_str(), sql.size(), &pStmt,  NULL );
	if( rc!=SQLITE_OK ){
           fprintf(stderr, "SQL error: %s\n", zErrMsg);
    	   sqlite3_close(db);

        }


    File *file=new File("../Der Blutige Pfad Gottes - German (DVD-Quali).avi");
//    File *file=new File("test.dvd");
//    PacketInputStream *pis=new PacketInputStream(NULL);
//    File *file=new File("test.avi");
    FormatInputStream *fis=new FormatInputStream(file);
    PacketInputStream *pis=new PacketInputStream(fis->getStream(0));
    Codec * codec=pis->getCodec();
    Packet packet;
    int duration=pis->getDuration();
//    cout << "Duration"<<duration<<endl;
//    Frame * frame;
    int count=0;
    sqlite3_exec(db,"BEGIN TRANSACTION",NULL,NULL,NULL);
    while(true&&count < 10000){
        packet=pis->readPacket();
        if(packet.data==NULL)break;
	if(++count%1000==0){
	    printf("\r%0.2f%%",(float)count/duration*100);
//	    cout<<dec<<setprecision(4) <<"\r"<<((float)count/duration*100);
//	    cout << 1.1;
	    flush(cout);
	}


//        unsigned char * buffer = new unsigned char[packet.size];
        sqlite3_bind_int( pStmt, 1, packet.stream_index);
        sqlite3_bind_int( pStmt, 2, packet.pts);
        sqlite3_bind_int( pStmt, 3, packet.dts);
        sqlite3_bind_int( pStmt, 4, packet.flags);
        sqlite3_bind_int( pStmt, 5, packet.duration);
        sqlite3_bind_int( pStmt, 6, packet.pos);
        sqlite3_bind_int( pStmt, 7, packet.size);
        sqlite3_bind_blob( pStmt, 8, (char*)packet.data,packet.size, SQLITE_STATIC );
        rc=sqlite3_step(pStmt);

        rc = sqlite3_reset(pStmt);
	if( rc!=SQLITE_OK ){
           fprintf(stderr, "SQL error: %s\n", zErrMsg);
	   sqlite3_free(zErrMsg);
        }
	
    }
    sqlite3_exec(db,"commit",NULL,NULL,NULL);
	delete fis;
	delete pis;
	delete file;
}


