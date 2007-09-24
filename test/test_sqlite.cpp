#include "org/esb/io/File.h"
#include "org/esb/av/FormatInputStream.h"
#include "org/esb/av/PacketInputStream.h"
#include "org/esb/av/Packet.h"
#include <sqlite3.h>
#include <iostream>

using namespace std;
using namespace org::esb::io;
using namespace org::esb::av;

int main(){

    sqlite3 *db;
    sqlite3_stmt *pStmt;
    char *zErrMsg = 0;
    int rc = sqlite3_open("/tmp/hive/test.db", &db);
    if( rc ){
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
	exit(1);
    }
//    string sql="insert into packet(id,intime,job,format,codec,width,height,packet_size,packet) values (NULL,NULL,1,?,?,?,?,?,?)";
    string sql="insert into packet(id,pts,dts,stream_index,flags,duration,pos,data_size,data) values (NULL,?,?,?,?,?,?,?,?)";
    sqlite3_prepare( db, sql.c_str(), sql.size(), &pStmt,  NULL );


    File *file=new File("../Der Blutige Pfad Gottes - German (DVD-Quali).avi");
//    File *file=new File("test.dvd");
//    PacketInputStream *pis=new PacketInputStream(NULL);
//    File *file=new File("test.avi");
    FormatInputStream *fis=new FormatInputStream(file);
    PacketInputStream *pis=new PacketInputStream(fis->getStream(0));
    Codec * codec=pis->getCodec();
    Packet packet;
//    Frame * frame;
    int count=0;
    sqlite3_exec(db,"BEGIN TRANSACTION",NULL,NULL,NULL);
    while(true&&count < 10000){
        packet=pis->readPacket();
        if(packet.data==NULL)break;
	if(++count%1000==0)cout << count << "Packets in db"<<endl;


//        unsigned char * buffer = new unsigned char[packet.size];
        sqlite3_bind_int( pStmt, 1, packet.pts);
        sqlite3_bind_int( pStmt, 2, packet.dts);
        sqlite3_bind_int( pStmt, 3, packet.stream_index);
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

}


