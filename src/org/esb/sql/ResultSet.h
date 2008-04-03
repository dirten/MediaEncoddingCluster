#ifndef ORG_ESB_SQL_RESULTSET_H
#define ORG_ESB_SQL_RESULTSET_H
#include "Statement.h"
#include "sqlite3x.hpp"
#include "tntdb/result.h"
#include "tntdb/row.h"
using namespace sqlite3x;
namespace org{
namespace esb{
namespace sql{

class ResultSet{
	public:
		ResultSet(tntdb::Result result);
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
		string getBlob(int col);
		string getBlob(string col);
	private:
		tntdb::Result _result;
		tntdb::Row _row;
		tntdb::Result::const_iterator * _rows;
//		int getColumnIndex(string name);
};
}}}
#endif

