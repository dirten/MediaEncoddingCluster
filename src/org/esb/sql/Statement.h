#ifndef ORG_ESB_SQL_STATEMENT_H
#define ORG_ESB_SQL_STATEMENT_H
#include <string>
#include "Connection.h"
//#include "ResultSet.h"
//#include "sqlite3x.hpp"
//#include "tntdb/statement.h"
#include <mysql/mysql.h>
#include "SqlException.h"
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
//		friend class ResultSet;
		Statement(Connection & db, const char * sql);
		~Statement();
//		Statement(tntdb::Statement &stmt);
//		Statement(const Statement & st);
//		ResultSet & executeQuery (string sql, void * callback);
/*
		void bind(int pos, string ref);
		void bind(int pos, int ref);
		void bind(int pos, const void * ref, int size);
*/
		ResultSet executeQuery ();
		void execute();
		void close();
	protected:
		
//		tntdb::Statement _stmt;
		Connection & _con;
		const char * _sql;
		friend class ResultSet;
		MYSQL_RES * _res;
		MYSQL_STMT* _stmt;

};
}}}
#endif


