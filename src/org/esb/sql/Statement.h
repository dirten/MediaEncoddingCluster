#ifndef ORG_ESB_SQL_STATEMENT_H
#define ORG_ESB_SQL_STATEMENT_H
#include <string>
#include <map>
#include "Connection.h"
#include <my_global.h>
#include <mysql.h>
#include "SqlException.h"
#include "org/esb/util/Log.h"
//using namespace std;
namespace org{
namespace esb{
namespace sql{
class Connection;
class ResultSet;
class Statement{
	logger("hive.sql.Statement")
	public:
		Statement(MYSQL & mysql, const char * sql);
		~Statement();
		ResultSet executeQuery ();
		ResultSet executeQuery(char * );
		bool execute();
		void close();
	protected:
		friend class ResultSet;
		MYSQL_STMT      *stmt;
        std::map<std::string,int> vars;
    private:
        std::string sql;

};
}}}
#endif


