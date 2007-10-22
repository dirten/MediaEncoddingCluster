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

	
}
