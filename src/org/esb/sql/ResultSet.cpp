#include "Column.h"
#include "ResultSet.h"

//#include "org/esb/util/Log.h"

namespace org{
namespace esb{
namespace sql{
/*******************************************************************************************************/
ResultSet::ResultSet(MYSQL_STMT * stmt){
  _rowPtr=boost::shared_ptr<Row>(new Row(stmt));
//  logdebug("ResultSet::ResultSet(MYSQL_STMT & stmt):row(&stmt)");
}
ResultSet::~ResultSet(){
//  logdebug("ResultSet::~ResultSet()");
}

/*******************************************************************************************************/
bool ResultSet::next(){
	return _rowPtr->next();
}

/*******************************************************************************************************/
const string ResultSet::getString(const int index){return _rowPtr->getColumn(index)->getString();}
const string ResultSet::getString(string index) {return _rowPtr->getColumn(index)->getString();}

/*******************************************************************************************************/
bool ResultSet::getBool(int index){return _rowPtr->getColumn(index)->getBool();}
bool ResultSet::getBool(string index){return _rowPtr->getColumn(index)->getBool();}

/*******************************************************************************************************/
int ResultSet::getInt(int index){return _rowPtr->getColumn(index)->getInt();}
int ResultSet::getInt(string index){return _rowPtr->getColumn(index)->getInt();}

/*******************************************************************************************************/
long long int ResultSet::getLong(int index){return _rowPtr->getColumn(index)->getLong();}
long long int ResultSet::getLong(string index){return _rowPtr->getColumn(index)->getLong();}

/*******************************************************************************************************/
double ResultSet::getDouble(int index){return _rowPtr->getColumn(index)->getDouble();}
double ResultSet::getDouble(string index){return _rowPtr->getColumn(index.c_str())->getDouble();}

/*******************************************************************************************************/
bool ResultSet::isNull(int index){return _rowPtr->getColumn(index)->isNull();}
bool ResultSet::isNull(string index){return _rowPtr->getColumn(index)->isNull();}

/*******************************************************************************************************/
string ResultSet::getBlob(int index){return _rowPtr->getColumn(index)->getBlob();}
string ResultSet::getBlob(string index){return _rowPtr->getColumn(index)->getBlob();}

/*******************************************************************************************************/
string ResultSet::getClob(int index){return getBlob(index);}
string ResultSet::getClob(string index){return getBlob(index);}

ResultSetMetaData * ResultSet::getResultSetMetaData(){
  return _rowPtr->getMetaData();
}
}}}


