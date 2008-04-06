#include "ResultSet.h"
#include "tntdb/blob.h"
//#include "tntdb/row.h"
#include <list.h>
using namespace org::esb::sql;
/*
ResultSet::ResultSet(boost::shared_ptr<MYSQL_RES> res){
	
	*_results=&*res;
//	_rows=new tntdb::Result::const_iterator(result.begin());
}
*/
ResultSet::ResultSet(MYSQL_RES* res){
	__results=res;
}

ResultSet::ResultSet(const ResultSet & rs){}

bool ResultSet::next(){
	bool hasNext=_record=mysql_fetch_row(__results);
	_lengths=mysql_fetch_lengths(__results);
	return hasNext;
}

string ResultSet::getString(int col){return _record[col]==NULL?"":_record[col];}
string ResultSet::getString(string col){return getString(getColumnIndex(col));}

int ResultSet::getInt(int col){return atoi(_record[col]!=NULL?_record[col]:"0");}
int ResultSet::getInt(string col){return getInt(getColumnIndex(col));}

long ResultSet::getLong(int col){return atol(_record[col]!=NULL?_record[col]:"0");}
long ResultSet::getLong(string col){return getLong(getColumnIndex(col));}

double ResultSet::getDouble(int col){return atol(_record[col]!=NULL?_record[col]:"0");}
double ResultSet::getDouble(string col){return getDouble(getColumnIndex(col));}

float ResultSet::getFloat(int col){return atof(_record[col]!=NULL?_record[col]:"0.0");}
float ResultSet::getFloat(string col){return getFloat(getColumnIndex(col));}

bool ResultSet::isNull(int col){}
bool ResultSet::isNull(string col){}

string ResultSet::getBlob(int col){
	return string(_record[col]!=NULL?_record[col]:"",_lengths[col]);
}

string ResultSet::getBlob(string col){
	return getBlob(getColumnIndex(col));
}

int ResultSet::getColumnIndex(string name){
	int cols=mysql_num_fields(__results);;
	int index=-1;
	MYSQL_FIELD *fields=mysql_fetch_fields(__results);
	for (int a=0;a<cols;a++){
		if(strcasecmp(name.c_str(),fields[a].name)==0){
			index=a;
		}
	}
	return index;
}

