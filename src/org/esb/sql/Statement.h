#ifndef ORG_ESB_SQL_STATEMENT_H
#define ORG_ESB_SQL_STATEMENT_H
#include <string>
#include "Connection.h"
//#include "ResultSet.h"
//#include "sqlite3x.hpp"
//#include "tntdb/statement.h"
#include <mysql/mysql.h>
#include "SqlException.h"
#include "org/esb/util/Log.h"
#include "tntdb/statement.h"
#include "tntdb/result.h"
using namespace std;
//using namespace org::esb::sql;
//using namespace sqlite3x;
namespace org{
namespace esb{
namespace sql{
class Connection;
class ResultSet;
class Statement{
	logger("hive.sql.Statement")
	public:
		Statement(tntdb::Statement  stmt);
		~Statement();
		ResultSet executeQuery ();
		ResultSet & executeQuery(char * );
		bool execute();
		void close();
	protected:
		friend class ResultSet;
		tntdb::Statement tntstmt;
		ResultSet * res;

};
}}}
#endif


