#include "PreparedStatement.h"
#include <sqlite3.h>

using namespace org::esb::sql;

void PreparedStatement::setBlob(int pos, void* data, long length){
	sqlite3_bind_blob( _pStmt, pos, (const char*)data,length, SQLITE_STATIC );
}
void PreparedStatement::setDouble(int pos, double val){}
void PreparedStatement::setInt(int pos, int val){}
void PreparedStatement::setNull(int pos){}
void PreparedStatement::setClob(int pos, void* val, long length){}
void PreparedStatement::setString(int pos, string val){}
bool PreparedStatement::execute(){}
ResultSet & PreparedStatement::executeQuery(){}
int PreparedStatement::executeUpdate(){}
