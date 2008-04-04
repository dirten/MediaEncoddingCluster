#include "ResultSet.h"
#include "tntdb/blob.h"
//#include "tntdb/row.h"
#include <list.h>
using namespace org::esb::sql;

ResultSet::ResultSet(tntdb::Result result):_rows(result.begin()){
	_result=result;
//	_rows=new tntdb::Result::const_iterator(result.begin());
}

//ResultSet::ResultSet(const ResultSet & rs){}

bool ResultSet::next(){
	++_rows;
	_row=*_rows;
	cout <<"isNull:"<< _result.getRow(1).isNull(1) << endl;
	if(_rows!=_result.end())
		return true;
	else
		return false;
}

string ResultSet::getString(int col){return _row.getString(col);}
string ResultSet::getString(string col){return _row.getString(col);}

int ResultSet::getInt(int col){return _row.getInt(col);}
int ResultSet::getInt(string col){return _row.getInt(col);}

long ResultSet::getLong(int col){return _row.getInt64(col);}
long ResultSet::getLong(string col){return _row.getInt64(col);}

double ResultSet::getDouble(int col){return _row.getDouble(col);}
double ResultSet::getDouble(string col){return _row.getDouble(col);}

bool ResultSet::isNull(int col){return _row.isNull(col);}
bool ResultSet::isNull(string col){return _row.isNull(col);}

string ResultSet::getBlob(int col){
	tntdb::Blob b=_row.getBlob(col);
	string blob(b.data(),b.size());
	return blob;
}
string ResultSet::getBlob(string col){
	tntdb::Blob b=_row.getBlob(col);
	string blob(b.data(),b.size());
	return blob;
}
/*
int ResultSet::getColumnIndex(string name){
	int cols=getColumnCount();
	int index=0;
	for (int a=0;a<cols;a++){
		cmd->stmt
		if(strcmp(name,sqlite3_column_name(cmd->stmt, a))==0){
			index=a;
		}
	}
	return index;
	
	
}
*/

