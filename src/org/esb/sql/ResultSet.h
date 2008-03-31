#ifndef ORG_ESB_SQL_RESULTSET_H
#define ORG_ESB_SQL_RESULTSET_H
#include "Statement.h"
#include "sqlite3x.hpp"
using namespace sqlite3x;
namespace org{
namespace esb{
namespace sql{

class ResultSet:public sqlite3_reader{
	public:
		ResultSet(Statement & stmt);
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
//	private:
//		int getColumnIndex(string name);
};
}}}
#endif

