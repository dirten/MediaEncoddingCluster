#ifndef ORG_ESB_SQL_STATEMENT_H
#define ORG_ESB_SQL_STATEMENT_H
#include <string>
//#include "Connection.h"
//#include "ResultSet.h"
#include "sqlite3x.hpp"

using namespace std;
//using namespace org::esb::sql;
using namespace sqlite3x;
namespace org{
namespace esb{
namespace sql{
class Connection;
class ResultSet;
class Statement:public sqlite3_command {
	public:
//		friend class ResultSet;
		Statement(Connection & db, const char * sql);
		Statement(const Statement & st);
//		ResultSet & executeQuery (string sql, void * callback);
		ResultSet executeQuery ();
		void execute();
		void close();
	private:	

};
}}}
#endif


