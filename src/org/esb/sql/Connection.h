#ifndef ORG_ESB_SQL_CONNECTION_H
#define ORG_ESB_SQL_CONNECTION_H
#include <mysql/mysql.h>
#include "org/esb/util/Log.h"
#include "SqlException.h"


namespace org{
namespace esb{
namespace sql{
class PreparedStatement;
class Statement;
class Connection{
	logger("hive.sql.Connection")
	public:
		Connection							(const char*connect_str)	throw (SqlException);
		~Connection							();
		Statement 			createStatement	(const char * sql);
		PreparedStatement 	prepareStatement(const char * sql);
		void 				executeNonQuery	(const char * sql);
		long 				lastInsertId	();
//		sqlite3_transaction getTransaction();
		void 				close();

		static const int AUTOCOMMIT=1;
		static const int USERCOMMIT=2;
	private:
		friend class Statement;
		friend class PreparedStatement;
		
	    Statement * _tmpStatement;
//	    tntdb::Connection _con;
		MYSQL * mysql;
};
}}}
#endif

