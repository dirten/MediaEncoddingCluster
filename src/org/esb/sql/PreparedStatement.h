#ifndef PREPAREDSTATEMENT_H_
#define PREPAREDSTATEMENT_H_
#include <string>
#include "Connection.h"
#include "ResultSet.h"
#include "org/esb/util/Log.h"
#include "tntdb/statement.h"
//#include "ResultSet.h"
//#include "ResultSet.h"
//#include "sqlite3x.hpp"
#include "mysql/mysql.h"
#include "mysql++.h"
#include "Column.h"
#include "Parameter.cpp"

using namespace std;
//using namespace sqlite3x;
namespace org{
	namespace esb{
		namespace sql{
			class Connection;
			class ResultSet;
			class PreparedStatement:public Statement{
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
				private:
                  Parameter * para;
//				  const char * parseSql(const char *);
				  map<int,Column*> col_vars;
//				  char * psql;
//				  std::string sql;
//					tntdb::Statement tntstmt;

			};
		}
	}
}
#endif /*PREPAREDSTATEMENT_H_*/
