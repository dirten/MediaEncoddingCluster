#include "PreparedStatement.h"
#include <iostream>
using namespace org::esb::sql;

PreparedStatement::PreparedStatement(Connection & db, const char * sql) : _con(db), _sql(sql){
    logdebug("PreparedStatement()"<<sql);
    _stmt=mysql_stmt_init(_con.mysql);
    _is_null=0;
    _is_error=0;
//    fprintf(stderr, "test bla fasel");
	if (!_stmt)	{
		throw SqlException("mysql_stmt_init(), out of memory");
	}
    if (mysql_stmt_prepare(_stmt, _sql, strlen(_sql))){
		throw SqlException(string("mysql_stmt_prepare failed").append(mysql_stmt_error(_stmt)));
    }
    _columnCount=mysql_stmt_param_count(_stmt);
    _is_null=new my_bool[_columnCount];
    _is_error=new my_bool[_columnCount];
//    _data_length=new int[_columnCount];
//    std::cout << "ColumnCount:"<<_columnCount<<std::endl;
    _col=new MYSQL_BIND[_columnCount];
    memset(_col, 0, sizeof(MYSQL_BIND)*_columnCount);

}

PreparedStatement::~PreparedStatement(){
    logdebug("~PreparedStatement");
    if(mysql_stmt_free_result(_stmt)){
	throw SqlException(string("free result failed: ").append(mysql_stmt_error(_stmt)));
    }
    if(mysql_stmt_close(_stmt)){
	throw SqlException(string("close statement failed: ").append(mysql_stmt_error(_stmt)));    
    }
    delete []_col;
    delete []_is_error;
    delete []_is_null;
    
//    _col=0;    
}

void PreparedStatement::setBlob(int pos, char* data, int length){
	bind(pos,MYSQL_TYPE_BLOB, data, length);
}

void PreparedStatement::setClob(int pos, char* data, int length){
	bind(pos,MYSQL_TYPE_STRING, data, length);
}

void PreparedStatement::setDouble(int pos, double data){
	setValue(pos, data,MYSQL_TYPE_DOUBLE);
//	bind(pos,MYSQL_TYPE_DOUBLE, (void*)&data,0 );
}

void PreparedStatement::setInt(int pos, int data){
	setValue(pos, data,MYSQL_TYPE_LONG);
//	bind(pos,MYSQL_TYPE_LONG, (void*)&data,sizeof(int));	
}

void PreparedStatement::setNull(int pos){

}

void PreparedStatement::setString(int pos, string & data){
	bind(pos,MYSQL_TYPE_VAR_STRING, (char*)data.c_str(), data.length());
}

void PreparedStatement::setString(int pos, char * data){
	if(data){
	    int length=strlen(data);
	    bind(pos,MYSQL_TYPE_VAR_STRING, (char*)data, length);
	}
}

bool PreparedStatement::execute(){
	logdebug("stmt bind param:"<<_sql);
	if (mysql_stmt_bind_param(_stmt,_col)){
	    throw SqlException(string("bind param failed: ").append(mysql_stmt_error(_stmt)));
	}

	logdebug("stmt execute:"<<_sql);
	if (mysql_stmt_execute(_stmt)){
	    throw SqlException(string("stmt execute failed: ").append(mysql_stmt_error(_stmt)));
	}
}

int PreparedStatement::executeUpdate(){

}

ResultSet PreparedStatement::executeQuery(){
//    mysql_stmt_bind_param(_stmt, _col);
//    mysql_stmt_execute(_stmt);
    execute();

//    mysql_use_result(_con);

    if (mysql_stmt_store_result(_stmt)){
        throw SqlException(string("stmt store result failed: ").append(mysql_stmt_error(_stmt)));
    }
    
    
//    mysql_stmt_store_result(_stmt);
//    int num_rows = mysql_stmt_num_rows(_stmt);
//    cout <<"numRows:"<<num_rows<<endl;
//    return ResultSet(_stmt);
}
unsigned long long PreparedStatement::getLastInsertId(){
    return mysql_stmt_insert_id(_stmt);
}

void PreparedStatement::bind(int col, enum_field_types buffer_type,char * data, int data_length){
    _col[col].buffer_type=buffer_type;
    _col[col].buffer=data;
    if(data_length>0)
	_is_null[col]=0;
    else
	_is_null[col]=1;
    _col[col].is_null=&_is_null[col];
    _col[col].error=&_is_error[col];
//    _col[col].length=(long unsigned int*)&_data_length[col];
    _col[col].buffer_length=(long unsigned int)data_length;
}

template <typename c_type>
void PreparedStatement::setValue(int col, c_type value, enum_field_types mysql_type)
    {
    	_col[col].buffer=new char[sizeof(c_type)];
    	_col[col].buffer_length=sizeof(c_type);
    	long unsigned int len=sizeof(c_type);
    	_col[col].length=(long unsigned int*)&len;
        *static_cast<c_type*>(_col[col].buffer) = value;
        _col[col].buffer_type = mysql_type;
//      _col[col].is_null = &_is_null;
//      _col[col].error=&_is_error;
    }

