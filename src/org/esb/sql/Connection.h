#ifndef ORG_ESB_SQL_CONNECTION_H
#define ORG_ESB_SQL_CONNECTION_H
//#include "org/esb/io/File.h"
//#include "Statement.h"
//#include "PreparedStatement.h"
#include "sqlite3x.hpp"
#include "tntdb/connection.h"
#include <mysql/mysql.h>
#include "boost/smart_ptr.hpp"
//using namespace org::esb::io;
using namespace sqlite3x;

namespace org{
namespace esb{
namespace io{
class File;
}}}
namespace org{
namespace esb{
namespace sql{
class PreparedStatement;
class Statement;
class Connection{
	public:
		Connection(char*connect_str);
//		Connection(char*filename);
		~Connection();
		Connection(org::esb::io::File & databaseFile);
		Statement createStatement(const char * sql);
//		Statement & createStatement();
		void executeNonQuery(const char * sql);
		long lastInsertId();
		PreparedStatement & prepareStatement(const char * sql);
		sqlite3_transaction getTransaction();
		void close();
		static const int AUTOCOMMIT=1;
		static const int USERCOMMIT=2;
	private:
		friend class Statement;
	    Statement * _tmpStatement;
	    tntdb::Connection _con;
		MYSQL * mysql;
};
}}}
#endif

