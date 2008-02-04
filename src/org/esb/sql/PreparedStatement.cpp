#include "PreparedStatement.h"
//#include <sqlite3.h>

using namespace org::esb::sql;
PreparedStatement::PreparedStatement(Connection & db, const char * sql)
    :sqlite3_command((sqlite3_connection&)db, sql){

}

void PreparedStatement::setBlob(int pos, void* data, int length){
	bind(pos, (const char*)data,length);
}

void PreparedStatement::setClob(int pos, void* data, int length){
	bind(pos, (const char*)data,length);
}

void PreparedStatement::setDouble(int pos, double data){bind(pos,data);}
void PreparedStatement::setInt(int pos, int data){bind(pos,data);}
void PreparedStatement::setNull(int pos){bind(pos,NULL);}
void PreparedStatement::setString(int pos, string data){bind(pos,data);}
bool PreparedStatement::execute(){executenonquery();}
ResultSet & PreparedStatement::executeQuery(){}
int PreparedStatement::executeUpdate(){}
