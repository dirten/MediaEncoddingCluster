#include "FrameHive.h"
using namespace org::esb::config;
/*****************************************************************************/
MYSQL *mysql;
MYSQL_STMT *stmt;
sqlite3 *db;
sqlite3_stmt *pStmt;
char *zErrMsg = 0;
int rc;

/*****************************************************************************/
static int callback(void *NotUsed, int argc, char **argv, char **azColName){
  int i;
  for(i=0; i<argc; i++){
    printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
  }
  printf("\n");
  return 0;
}



/*****************************************************************************/
FrameHive::FrameHive(string dbname){

	this->frameCounter=0;
	this->pathCounter=0;
	this->pfad="/tmp/frame.container";
        this->compressor=new FrameCompressor();
	this->pFileHive=fopen("/tmp/frame.container/hive.data", "wb");
        
    rc = sqlite3_open(dbname.c_str(), &db);
    if( rc ){
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(1);
    }
    string sql="insert into test(data) values (?)";
    sqlite3_prepare( db, sql.c_str(), sql.size(), &pStmt,  NULL );


    if(Config::getConfig("mysql")=="true"){
    mysql = mysql_init(NULL);
    if(mysql == NULL) {
	fprintf(stderr, " Initialisierung fehlgeschlagen\n");
    }
        /* Mit dem Server verbinden */
    if( mysql_real_connect (
	mysql,   /* Zeiger auf MYSQL-Handler*/
        "10.122.6.163", /* Host-Name*/
        "root", /* User-Name*/
        NULL, /* Passwort für user_name */
        NULL,  /* Name der Datenbank*/
        0,     /* Port (default=0) */
        NULL,  /* Socket (default=NULL)*/
        0      /* keine Flags */  )  == NULL) {
       fprintf (stderr, "Fehler mysql_real_connect():" "%u (%s)\n",mysql_errno (mysql), mysql_error (mysql));
    }else{
	printf("Erfolgreich mit dem MySQL-Server verbunden\n");
    }
    int rc=0;
    rc=mysql_select_db(mysql,"framehive");
    if(rc!=0){
       fprintf (stderr, "Fehler mysql_select_db:" "%u (%s)\n",mysql_errno (mysql), mysql_error (mysql));    
    }
    stmt=mysql_stmt_init(mysql);
//    string sql="insert into test(data) values (?)";
    rc=mysql_stmt_prepare(stmt,sql.c_str(), sql.length());
    }

//
}


/*****************************************************************************/
FrameHive::~FrameHive(){
    sqlite3_close(db);
    /* Verbindung trennen */
    mysql_close (mysql);

}

/*****************************************************************************/
void FrameHive::putFrame( AVFrame * frame, AVCodecContext *codecCtx ){
//    this->putFrameHive(frame, codecCtx);
//    this->putFrameSQLite(frame, codecCtx);
//    this->putFrameMySQL(frame, codecCtx);
    this->putFrameFS(frame, codecCtx);
}

/*****************************************************************************/
void FrameHive::putFrameHive( AVFrame * frame, AVCodecContext *codecCtx ){
    if(COMPRESSED){
	unsigned char * data=new unsigned char[512*256*3];
        int filelen=compressor->deflateFrame(frame, data);
        fwrite(data, 1, filelen, pFileHive);
        delete data;
    }else{
	for(int y=0; y<codecCtx->height; y++)
	    fwrite(frame->data[0]+y*frame->linesize[0], 1, codecCtx->width*3, pFileHive);
	fflush(pFileHive);
    }
}
void FrameHive::putFrameMySQL( AVFrame * frame, AVCodecContext *codecCtx ){

    int height=codecCtx->height;
    int width=codecCtx->width;
    int bufSize=width*height*3;    
    unsigned char * buffer = new unsigned char[bufSize];
    bufSize=compressor->deflateFrame(frame, buffer);
/*
    for(int y=0; y<height; y++){
      memcpy(buffer+(frame->linesize[0]*y),frame->data[0]+(y*frame->linesize[0]),width*3);
    }
*/

    /* Hier befindet sich der Code für die Arbeit mit MySQL */
    MYSQL_BIND bind[1];
    memset(bind, 0, sizeof(bind));
    bind[0].buffer_type=MYSQL_TYPE_BLOB;
    bind[0].buffer=buffer;
    bind[0].buffer_length=bufSize;
//    bind[0].length=bufSize;
    my_bool code=mysql_stmt_bind_param(stmt,bind);
    int rc=mysql_stmt_execute(stmt);
    if(rc!=0){
       fprintf (stderr, "Fehler in execute:" "%u (%s)\n",mysql_errno (mysql), mysql_error (mysql));    
    }
    delete buffer;
}

/*****************************************************************************/
void FrameHive::putFrameFS( AVFrame * frame, AVCodecContext *codecCtx ){
  FILE *pFile;
  int bufSize=codecCtx->width*3;
  char buf[bufSize];
  int bzerror;
  int     nWritten;
  unsigned int nBytesIn;
  unsigned int nBytesOut;

  char szFilename[64];
  int  y;
    // Open file
    if(this->frameCounter%1000==0){
	++this->pathCounter;
	char newPfad[64];
	sprintf(newPfad, "%s/%d",pfad.c_str(), this->pathCounter);
	mkdir(newPfad,0755);
    }
  sprintf(szFilename, "%s/%d/frame%d.fstream.ppm",pfad.c_str(),pathCounter, ++frameCounter);
//  cout <<szFilename<<endl;

  pFile=fopen(szFilename, "w+b");
  if(pFile==NULL)
    return;
  fprintf(pFile, "P6\n%d %d\n255\n", codecCtx->width, codecCtx->height);
  for(y=0; y<codecCtx->height; y++)
    fwrite(frame->data[0]+y*frame->linesize[0], 1, codecCtx->width*3, pFile);
  fclose(pFile);
  
/*
    string header="P6\n";
    header+="512 256";
//    header+="\n";
//    header+="256";
    header+="\n";
    header+="255\n";
    fstream file(szFilename, ios::out|ios::binary);
    file.write(header.c_str(), header.size());
    file.write((const char*)frame->data[0], (codecCtx->width*codecCtx->width*3));
    file.close();
*/    
}

/*****************************************************************************/
void FrameHive::putFrameSQLite( AVFrame * frame, AVCodecContext *codecCtx ){
    int height=codecCtx->height;
    int width=codecCtx->width;
    int bufSize=width*height*3;    
    unsigned char * buffer = new unsigned char[bufSize];
    for(int y=0; y<height; y++){
      memcpy(buffer+(frame->linesize[0]*y),frame->data[0]+(y*frame->linesize[0]),width*3);
    }
    sqlite3_bind_text( pStmt, 1, (char*)buffer,bufSize, SQLITE_STATIC );
    rc=sqlite3_step(pStmt);
    rc = sqlite3_reset(pStmt);
    if( rc!=SQLITE_OK ){
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
    }
    delete buffer;
}

