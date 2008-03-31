#include "ResultSet.h"

using namespace org::esb::sql;

ResultSet::ResultSet(Statement & stmt):sqlite3_reader(stmt.executereader()){}

ResultSet::ResultSet(const ResultSet & rs):sqlite3_reader(rs){}

bool ResultSet::next(){return read();}

string ResultSet::getString(int col){return getstring(col);}
string ResultSet::getString(string col){return getString(getColumnIndex(col));}

int ResultSet::getInt(int col){return getint(col);}
int ResultSet::getInt(string col){return getInt(getColumnIndex(col));}

long ResultSet::getLong(int col){return getint64(col);}
long ResultSet::getLong(string col){return getLong(getColumnIndex(col));}

string ResultSet::getBlob(int col){return getblob(col);}
string ResultSet::getBlob(string col){return getBlob(getColumnIndex(col));}
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

