#include "ResultSet.h"
#include <list>
using namespace org::esb::sql;

namespace org{
namespace esb{
namespace sql{
class Column{
    public:
	Column(MYSQL_FIELD & field){
	    type=field.type;
	    unsigned int flag=field.flags;
	    data=new char[255];
	}
	enum_field_types type;
	void * data;
	unsigned long length;
	my_bool is_null;
	my_bool is_error;
};
}}}
ResultSet::ResultSet(Statement & stmt):_stmt(stmt){
    _resultSetMetadata=mysql_stmt_result_metadata(_stmt._stmt);

if (!_resultSetMetadata){
    fprintf(stderr," mysql_stmt_result_metadata(), returned no meta information\n");
    fprintf(stderr, " %s\n", mysql_stmt_error(_stmt._stmt));
    exit(0);
}

    int _columnCount=mysql_num_fields(_resultSetMetadata);
    MYSQL_FIELD * fields=mysql_fetch_fields(_resultSetMetadata);
       
    _bindColumns=new MYSQL_BIND[_columnCount];
    memset(_bindColumns, 0, sizeof(MYSQL_BIND)*_columnCount);

    for(int i = 0; i < _columnCount; i++){
	Column * col=new Column(fields[i]);
	_row.push_back(col);
	_bindColumns[i].buffer_type=col->type;
	_bindColumns[i].buffer=col->data;
	_bindColumns[i].buffer_length=col->length;
	_bindColumns[i].is_null=&col->is_null;
	_bindColumns[i].error=&col->is_error;

	printf("Field %u is %s\n", i, fields[i].name);
    }
    if (mysql_stmt_bind_result(_stmt._stmt, _bindColumns)){
        fprintf(stderr, " mysql_stmt_bind_result() failed\n");
        fprintf(stderr, " %s\n", mysql_stmt_error(_stmt._stmt));
        exit(0);
    }
}

bool ResultSet::next(){	
	bool hasNext=(!mysql_stmt_fetch(_stmt._stmt));
	return hasNext;
}

string ResultSet::getString(int col){return static_cast<const char*>(_bindColumns[col].buffer);}
string ResultSet::getString(string col){return getString(getColumnIndex(col));}

int ResultSet::getInt(int col){return *static_cast<short int*>(_row[col]->data);}
int ResultSet::getInt(string col){return getInt(getColumnIndex(col));}

long ResultSet::getLong(int col){}
long ResultSet::getLong(string col){return getLong(getColumnIndex(col));}

double ResultSet::getDouble(int col){}
double ResultSet::getDouble(string col){return getDouble(getColumnIndex(col));}

float ResultSet::getFloat(int col){}
float ResultSet::getFloat(string col){return getFloat(getColumnIndex(col));}

bool ResultSet::isNull(int col){}
bool ResultSet::isNull(string col){return isNull(getColumnIndex(col));}

string ResultSet::getBlob(int col){}

string ResultSet::getBlob(string col){
	return getBlob(getColumnIndex(col));
}

int ResultSet::getColumnIndex(string name){
	int cols=mysql_num_fields(_resultSetMetadata);
	int index=-1;
	MYSQL_FIELD *fields=mysql_fetch_fields(_resultSetMetadata);
	for (int a=0;a<cols;a++){
		if(strcasecmp(name.c_str(),fields[a].name)==0){
			index=a;
		}
	}
	return index;
}

