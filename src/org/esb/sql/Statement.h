#ifndef ORG_ESB_SQL_STATEMENT_H
#define ORG_ESB_SQL_STATEMENT_H
#include <string>
#include "ResultSet.h"
#include <sqlite3.h>


using namespace std;
using namespace org::esb::sql;

namespace org{
namespace esb{
namespace sql{

class Statement {
	public:
		Statement(sqlite3 * db);
		ResultSet & executeQuery (string sql, void * callback);
		void execute(string sql);
		void close();
	private:	
		sqlite3 * _db;

};
}}}
#endif


