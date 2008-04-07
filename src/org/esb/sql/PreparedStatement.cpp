#include "PreparedStatement.h"

using namespace org::esb::sql;

PreparedStatement::PreparedStatement(Connection & db, const char * sql) : _con(db), _sql(sql){
    _stmt=mysql_stmt_init(_con.mysql);
    _is_null=0;
    _is_error=0;
//    fprintf(stderr, "test bla fasel");
	if (!_stmt)
	{
	  fprintf(stderr, " mysql_stmt_init(), out of memory\n");
	    exit(0);
	}
if (mysql_stmt_prepare(_stmt, _sql, strlen(_sql)))
{
  fprintf(stderr, " mysql_stmt_prepare(), INSERT failed\n");
    fprintf(stderr, " %s\n", mysql_stmt_error(_stmt));
      exit(0);
      }
//	mysql_stmt_prepare(_stmt,_sql, strlen(_sql));
	_columnCount=mysql_stmt_param_count(_stmt);
//	cout << "ColumnCount:"<<_columnCount<<endl;
	_col=new MYSQL_BIND[_columnCount];
	memset(_col, 0, sizeof(MYSQL_BIND[_columnCount]));
}

PreparedStatement::~PreparedStatement(){
    delete []_col;
}

void PreparedStatement::setBlob(int pos, void* data, int length){
	bind(pos,MYSQL_TYPE_BLOB, data, length);	
}

void PreparedStatement::setClob(int pos, void* data, int length){
	bind(pos,MYSQL_TYPE_STRING, data, length);	
}

void PreparedStatement::setDouble(int pos, double data){
	bind(pos,MYSQL_TYPE_DOUBLE, (void*)&data,0 );	
}

void PreparedStatement::setInt(int pos, int data){
	setValue(pos, data,MYSQL_TYPE_LONG);
//	bind(pos,MYSQL_TYPE_LONG, (void*)&data,sizeof(int));	
}

void PreparedStatement::setNull(int pos){

}

void PreparedStatement::setString(int pos, string data){
	bind(pos,MYSQL_TYPE_STRING, (void*)data.c_str(), data.length());
}

bool PreparedStatement::execute(){
	if (mysql_stmt_bind_param(_stmt,_col)){
		fprintf(stderr, " mysql_stmt_bind_param() failed\n");
		fprintf(stderr, " %s\n", mysql_stmt_error(_stmt));
		exit(0);
	}
	if (mysql_stmt_execute(_stmt)){
		fprintf(stderr, " mysql_stmt_execute(), 1 failed\n");
		fprintf(stderr, " %s\n", mysql_stmt_error(_stmt));
		exit(0);
	}

//    mysql_stmt_bind_param(_stmt, _col);
//    mysql_stmt_execute(_stmt);
}

int PreparedStatement::executeUpdate(){

}

ResultSet PreparedStatement::executeQuery(){
    mysql_stmt_bind_param(_stmt, _col);
    mysql_stmt_execute(_stmt);
    mysql_stmt_store_result(_stmt);
    int num_rows = mysql_stmt_num_rows(_stmt);
//    cout <<"numRows:"<<num_rows<<endl;
    return ResultSet(*this->_stmt);
}

void PreparedStatement::bind(int col, enum_field_types buffer_type,void * data, int data_length){
    _col[col].buffer_type=buffer_type;
    _col[col].buffer=data;
    _col[col].is_null=&_is_null;;
    _col[col].error=&_is_error;
    _col[col].length=(long unsigned int*)&data_length;
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

