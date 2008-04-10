#include "ResultSet.h"
#include "sstream"
#include <iostream>
#include <list>
#include "tntdb/bits/blob.h"
using namespace org::esb::sql;
using namespace std;
namespace org{
namespace esb{
namespace sql{
/*******************************************************************************************************/

/*******************************************************************************************************/
ResultSet::ResultSet(tntdb::Result result):tntresult(result),tntiterator(result.begin()){}


/*******************************************************************************************************/
bool ResultSet::next(){
	tntrow=*tntiterator;
	bool islast=tntiterator!=tntresult.end();
	tntiterator++;
	return islast;
}

/*******************************************************************************************************/
string ResultSet::getString(int col){return tntrow.getString(col);}
string ResultSet::getString(string col){return tntrow.getString(col);}


/*******************************************************************************************************/
    bool ResultSet::getBool(int index){return tntrow.getBool(index);}
    bool ResultSet::getBool(string index){return tntrow.getBool(index);}

/*******************************************************************************************************/
    int ResultSet::getInt(int index){return tntrow.getInt(index);}
    int ResultSet::getInt(string index){return tntrow.getInt(index);}

/*******************************************************************************************************/
    double ResultSet::getDouble(int index){return tntrow.getDouble(index);}
    double ResultSet::getDouble(string index){return tntrow.getDouble(index);}

/*******************************************************************************************************/
bool ResultSet::isNull(int col){return tntrow.isNull(col);}
bool ResultSet::isNull(string col){return tntrow.isNull(col);}

    string ResultSet::getBlob(int index){
    	tntdb::Blob lob=tntrow.getBlob(index);
    	return string(lob.data(), lob.size());
    }
    string ResultSet::getBlob(string index){
    	tntdb::Blob lob=tntrow.getBlob(index);
    	return string(lob.data(), lob.size());
    }

string ResultSet::getClob(int col){return getBlob(col);}
string ResultSet::getClob(string col){return getBlob(col);}
}}}