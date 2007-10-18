
#include <sqlite3.h>
#include <iostream>
#include "org/esb/io/File.h"
#include "org/esb/av/FormatInputStream.h"
#include "org/esb/av/PacketInputStream.h"
#include "org/esb/av/Packet.h"
#include "org/esb/av/Codec.h"
#include "CreateDatabase.cpp"

using namespace std;
using namespace org::esb::io;
using namespace org::esb::av;

int main(int argc, char * argv[]){

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

    string sql="insert into packets(id,pts,dts,stream_index,flags,duration,pos,data_size,data) values (NULL,?,?,?,?,?,?,?,?)";

    sqlite3_prepare( db, sql.c_str(), sql.size(), &pStmt,  NULL );
    sqlite3_exec(db,"BEGIN TRANSACTION",NULL,NULL,NULL);


    FormatInputStream fis(&inputFile);
	int streamCount=fis.getStreamCount();
	cout << "StreamCount="<<streamCount<<endl;
//	for(int a=0;a<streamCount;a++){
//	cout << "Loading Stream "<<a<<endl; 
    PacketInputStream pis(&fis);
//    Codec * codec=pis.getCodec();
    Packet packet;




	int count=0;
    while(true&&count < 2000){
        packet=pis.readPacket();
        if(packet.data==NULL)break;
		if(++count%1000==0)cout << count << "Packets in db"<<endl;


        sqlite3_bind_int( pStmt, 1, packet.pts);
        sqlite3_bind_int( pStmt, 2, packet.dts);
        sqlite3_bind_int( pStmt, 3, packet.stream_index);
        sqlite3_bind_int( pStmt, 4, packet.flags);
        sqlite3_bind_int( pStmt, 5, packet.duration);
        sqlite3_bind_int( pStmt, 6, packet.pos);
        sqlite3_bind_int( pStmt, 7, packet.size);
        sqlite3_bind_blob( pStmt, 8, (char*)packet.data,packet.size, SQLITE_STATIC );
        int rc=sqlite3_step(pStmt);

        rc = sqlite3_reset(pStmt);
		if( rc!=SQLITE_OK ){
           fprintf(stderr, "SQL error: %s\n", zErrMsg);
	   		sqlite3_free(zErrMsg);
        }
    }
 //   }
    sqlite3_exec(db,"commit",NULL,NULL,NULL);

 sqlite3_close(db);
//free(db);


	return 0;
}
