#include "ResultSet.h"
#include "sstream"
#include <iostream>
#include <list>
#include "tntdb/bits/blob.h"
//#include "Row.cpp"
#include "Column.h"
using namespace org::esb::sql;
using namespace std;
namespace org{
namespace esb{
namespace sql{
/*******************************************************************************************************/

/*******************************************************************************************************/

ResultSet::ResultSet(Statement & stmt):cppstmt(NULL),row(stmt.stmt){
}



/*******************************************************************************************************/
bool ResultSet::next(){
	return row.next();
}

/*******************************************************************************************************/
string ResultSet::getString(int col){return string(cpprow[col]);}
string ResultSet::getString(string col){return string(row.getColumn(col.c_str())->getString());}


/*******************************************************************************************************/
bool ResultSet::getBool(int index){return tntrow.getBool(index);}
bool ResultSet::getBool(string index){return row.getColumn(index.c_str())->getBool();}

/*******************************************************************************************************/
int ResultSet::getInt(int index){return tntrow.getInt(index);}
int ResultSet::getInt(string index){return row.getColumn(index.c_str())->getInt();}

/*******************************************************************************************************/
double ResultSet::getDouble(int index){return tntrow.getDouble(index);}
double ResultSet::getDouble(string index){return row.getColumn(index.c_str())->getDouble();}

/*******************************************************************************************************/
bool ResultSet::isNull(int col){return tntrow.isNull(col);}
bool ResultSet::isNull(string col){return row.getColumn(col.c_str())->isNull();}

string ResultSet::getBlob(int index){
    	tntdb::Blob lob=tntrow.getBlob(index);
    	return string(lob.data(), lob.size());
//    	return row.getColumn(index.c_str())->getBlob();
    }
string ResultSet::getBlob(string index){
//    	tntdb::Blob lob=tntrow.getBlob(index);
//    	return string(lob.data(), lob.size());
    	return row.getColumn(index.c_str())->getBlob();
    }

string ResultSet::getClob(int col){return getBlob(col);}
string ResultSet::getClob(string col){return getBlob(col);}
}}}