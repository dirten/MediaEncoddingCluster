#ifndef PREPAREDSTATEMENT_H_
#define PREPAREDSTATEMENT_H_
#include <string>
#include "Connection.h"
#include "ResultSet.h"
#include "org/esb/util/Log.h"
//#include "tntdb/statement.h"
//#include "ResultSet.h"
//#include "ResultSet.h"
//#include "sqlite3x.hpp"
#ifdef MSVC
#include <my_global.h>
#endif

//#include <my_global.h>
#include <mysql.h>
//#include "mysql++.h"
#include "Column.h"
#include "Parameter.cpp"

using namespace std;
//using namespace sqlite3x;
namespace org{
	namespace esb{
		namespace sql{
			class Connection;
			class ResultSet;
			class PreparedStatement{
			    logger("hive.sql")
				public:
				    PreparedStatement(MYSQL & mysql, const char * sql);
//				    PreparedStatement(mysqlpp::Query stmt);
				    ~PreparedStatement();
					void setDouble(string pos, double val);
					void setInt(string pos, int val);
					void setNull(string pos);
					void setBlob(string pos, char* data, int length);
					void setClob(string pos, char* data, int length);
					void setString(string pos, string val);
					void setString(string pos, char * val);
					unsigned long long getLastInsertId();
					int executeUpdate();
					bool execute();
					ResultSet executeQuery();
					void close(void);
				private:
                  Parameter * para;
				  void parseSql(const char *);
				  map<int,Column*> col_vars;
				  map<std::string,int> vars;

                  MYSQL_STMT *stmt;
//				  char * psql;
				  std::string sql;
//					tntdb::Statement tntstmt;

			};
		}
	}
}
#endif /*PREPAREDSTATEMENT_H_*/
