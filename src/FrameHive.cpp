#include "FrameHive.h"

sqlite3 *db;
sqlite3_stmt *pStmt;
char *zErrMsg = 0;
int rc;

static int callback(void *NotUsed, int argc, char **argv, char **azColName){
  int i;
  for(i=0; i<argc; i++){
    printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
  }
  printf("\n");
  return 0;
}



FrameHive::FrameHive(string dbname){
        
    rc = sqlite3_open(dbname.c_str(), &db);
    if( rc ){
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(1);
    }
    string sql="insert into test(data) values (?)";
    sqlite3_prepare( db, sql.c_str(), sql.size(), &pStmt,  NULL );
//
}


FrameHive::~FrameHive(){
    sqlite3_close(db);
}

void FrameHive::putFrame( AVFrame * frame, AVCodecContext *codecCtx ){
//    rc = sqlite3_exec(db, "insert into test(id, data) values (?,?)", callback, 0, &zErrMsg);
    string data="";//(const char * )frame->data[0];
    for(int y=0; y<codecCtx->height; y++){
//	string tmp=(const char * )frame->data[0];
//	strcpy(tmp,frame->data[0],codecCtx->width*3);
//	string tmp=
	data+=(const char *)frame->data[0]+y*frame->linesize[0];
    }
//	fwrite(frame->data[0]+y*frame->linesize[0], 1, codecCtx->width*3, pFile);

    sqlite3_bind_text( pStmt, 1, data.c_str(),data.size(), SQLITE_STATIC );
    sqlite3_step(pStmt);
    rc = sqlite3_reset(pStmt);
    if( rc!=SQLITE_OK ){
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
    }
}

