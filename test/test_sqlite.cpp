#include "org/esb/io/File.h"
//#include "org/esb/sql/Connection.h"
#include "org/esb/av/FormatInputStream.h"
#include "org/esb/av/PacketInputStream.h"
#include "org/esb/av/Packet.h"
#include "org/esb/av/Encoder.h"
#include <sqlite3.h>
#include <iostream>
#include <iomanip>
//#include "org/esb/sql/Connection.h"
//#include "org/esb/sql/Statement.h"
using namespace org::esb::av;
//using namespace sqlite3;
int main(){
	Encoder e(CODEC_ID_MP2);
	e.setSampleRate(44100);
	e.setBitRate(128000);
	e.setChannels(2);
	e.setSampleFormat((SampleFormat)1);
	e.time_base.num=0;
	e.time_base.den=0;
	e.open();
	cout << e.time_base.num<<":"<<e.time_base.den;
	cout << av_rescale_q(19097,(AVRational){1,15963},(AVRational){1,15963})<<endl;	
	cout << av_rescale_q(1,(AVRational){1,25},(AVRational){1,25})<<endl;	
//	cout << av_rescale_q(19097,15963,15963).den<<endl;	

}

/*
int main(){
//    int ts=sqlite3_threadsafe();
//    if(ts>0)cout << "ThreadSafe"<<endl;
//    File f("/tmp/hive.db");
//    Connection con(f);

//	cout << "Activate job"<<_id<<endl;
	int nResult;
	sscanf("86017", "%d", &nResult);
	av_register_all();
	avcodec_register_all();
	
	enum tmp{a=86017} t;
	cout << "Hex"<<0x15001<<":"<<t<<endl;
	AVCodec * codec=avcodec_find_encoder((CodecID)a);
	if(!codec)cout << "Codec not found"<<endl;
	Encoder * e=new Encoder((CodecID)a);
//	cout << "Encoder:"<<e->getCodecId()<<endl;

return 0;
}
*/
/*
using namespace std;
using namespace org::esb::io;
using namespace org::esb::av;

static int callback(void *NotUsed, int argc, char **argv, char **azColName){
	int i;
  for(i=0; i<argc; i++){
    printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
  }
  printf("\n");
  return 0;
}

int main(){
	File dbfile("./test.db2");
	Connection con(dbfile);
	Statement  *stmt = &con.createStatement();
	stmt->executeQuery("select count(*) from packets", (void*)callback);
//	delete &stmt;
	exit(0);


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


*/


