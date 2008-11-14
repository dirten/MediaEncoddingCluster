#include "ResultSet.h"
#include "Column.h"

namespace org{
namespace esb{
namespace sql{
/*******************************************************************************************************/
ResultSet::ResultSet(MYSQL_STMT & stmt):row(&stmt){
  logdebug("ResultSet::ResultSet(MYSQL_STMT & stmt):row(&stmt)");
}
ResultSet::~ResultSet(){
  logdebug("ResultSet::~ResultSet()");
}

/*******************************************************************************************************/
bool ResultSet::next(){
	return row.next();
}

/*******************************************************************************************************/
const string ResultSet::getString(const int index){return row.getColumn(index)->getString();}
const string ResultSet::getString(string index) {return row.getColumn(index)->getString();}

/*******************************************************************************************************/
bool ResultSet::getBool(int index){return row.getColumn(index)->getBool();}
bool ResultSet::getBool(string index){return row.getColumn(index)->getBool();}

/*******************************************************************************************************/
int ResultSet::getInt(int index){return row.getColumn(index)->getInt();}
int ResultSet::getInt(string index){return row.getColumn(index)->getInt();}

/*******************************************************************************************************/
double ResultSet::getDouble(int index){return row.getColumn(index)->getDouble();}
double ResultSet::getDouble(string index){return row.getColumn(index.c_str())->getDouble();}

/*******************************************************************************************************/
bool ResultSet::isNull(int index){return row.getColumn(index)->isNull();}
bool ResultSet::isNull(string index){return row.getColumn(index)->isNull();}

/*******************************************************************************************************/
string ResultSet::getBlob(int index){return row.getColumn(index)->getBlob();}
string ResultSet::getBlob(string index){return row.getColumn(index)->getBlob();}

/*******************************************************************************************************/
string ResultSet::getClob(int index){return getBlob(index);}
string ResultSet::getClob(string index){return getBlob(index);}

ResultSetMetaData * ResultSet::getResultSetMetaData(){
  return row.getMetaData();
}
}}}


