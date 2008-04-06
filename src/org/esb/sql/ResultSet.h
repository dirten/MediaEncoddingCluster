#ifndef ORG_ESB_SQL_RESULTSET_H
#define ORG_ESB_SQL_RESULTSET_H
#include "Statement.h"
#include "sqlite3x.hpp"
#include "tntdb/result.h"
#include "tntdb/row.h"
#include <mysql/mysql.h>
#include "boost/shared_ptr.hpp"
using namespace sqlite3x;
namespace org{
namespace esb{
namespace sql{

class ResultSet{
	public:
//		ResultSet(boost::shared_ptr<MYSQL_RES> res);
		ResultSet(MYSQL_RES* res);
		ResultSet(const ResultSet & rs);
		bool next();
		string getString(int col);
		string getString(string col);
		int getInt(int col);
		int getInt(string col);
		long getLong(int col);
		long getLong(string col);
		double getDouble(int col);
		double getDouble(string col);
		float getFloat(int col);
		float getFloat(string col);
		string getBlob(int col);
		string getBlob(string col);
		bool isNull(int col);
		bool isNull(string col);
	private:
		boost::shared_ptr<MYSQL_RES*> _results;
		MYSQL_RES* __results;
		MYSQL_ROW _record;
		unsigned long * _lengths;
		int getColumnIndex(string name);
};
}}}
#endif

