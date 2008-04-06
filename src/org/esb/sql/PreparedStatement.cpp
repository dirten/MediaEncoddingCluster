#include "PreparedStatement.h"
//#include <sqlite3.h>

using namespace org::esb::sql;
PreparedStatement::PreparedStatement(Connection & db, const char * sql):_con(db), _sql(sql){
	_stmt=mysql_stmt_init(_con.mysql);
	mysql_stmt_prepare(_stmt,_sql, strlen(_sql));
	_columnCount=mysql_stmt_param_count(_stmt);
	_col=new MYSQL_BIND[_columnCount];
}

void PreparedStatement::setBlob(int pos, void* data, int length){
	bind(pos,MYSQL_TYPE_STRING, data, length);	
}

void PreparedStatement::setClob(int pos, void* data, int length){
	bind(pos,MYSQL_TYPE_STRING, data, length);	
}

void PreparedStatement::setDouble(int pos, double data){
	bind(pos,MYSQL_TYPE_SHORT, (void*)&data,0 );	
}

void PreparedStatement::setInt(int pos, int data){
	bind(pos,MYSQL_TYPE_SHORT, (void*)&data,0 );	
}

void PreparedStatement::setNull(int pos){

}

void PreparedStatement::setString(int pos, string data){
	bind(pos,MYSQL_TYPE_STRING, (void*)data.c_str(), data.length());	
}

bool PreparedStatement::execute(){

}

int PreparedStatement::executeUpdate(){

}

void PreparedStatement::bind(int col, enum_field_types buffer_type,void * data, int data_length){
		_col[col].buffer_type= buffer_type;
		_col[col].buffer= data;
		_col[col].is_null= 0;
		_col[col].length=(long unsigned int*)&data_length;

}

