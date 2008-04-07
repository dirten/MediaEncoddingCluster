#ifndef ORG_ESB_SQL_RESULTSET_H
#define ORG_ESB_SQL_RESULTSET_H
#include "Statement.h"
#include "PreparedStatement.h"
#include "sqlite3x.hpp"
#include <mysql/mysql.h>
#include "boost/shared_ptr.hpp"
#include <vector>
using namespace sqlite3x;
namespace org{
namespace esb{
namespace sql{
class Column;
class ResultSet{
	public:
//		ResultSet(boost::shared_ptr<MYSQL_RES> res);
		ResultSet(Statement & stmt);
//		ResultSet(PreparedStatement & stmt);
//		ResultSet(MYSQL_RES* res);
//		ResultSet(const ResultSet & rs);
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
//		boost::shared_ptr<MYSQL_RES*> _results;
		Statement & _stmt;
//		PreparedStatement & _pstmt;
		MYSQL_RES* _resultSetMetadata;
		MYSQL_ROW _record;
		MYSQL_BIND * _bindColumns;
		unsigned long * _lengths;
		int getColumnIndex(string name);
		char data[255];
		my_bool isNullB;
		my_bool isError;
		unsigned long length;
		vector<Column*> _row;
		template <typename int_type> int_type getInteger(const MYSQL_BIND& bind);

};
}}}
#endif

