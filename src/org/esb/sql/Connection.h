#ifndef ORG_ESB_SQL_CONNECTION_H
#define ORG_ESB_SQL_CONNECTION_H
#include "org/esb/io/File.h"
#include "Statement.h"
#include "PreparedStatement.h"
#include "sqlite3x.hpp"

using namespace org::esb::io;
using namespace sqlite3x;


namespace org{
namespace esb{
namespace sql{
class Connection:public sqlite3_connection {
	public:
		Connection(char*filename);
		Connection(File & databaseFile);
		Statement createStatement(const char * sql);
		Statement & createStatement();
		PreparedStatement & prepareStatement(const char * sql);
		sqlite3_transaction getTransaction();
		void close();
		static const int AUTOCOMMIT=1;
		static const int USERCOMMIT=2;
	private:
	    Statement * _tmpStatement;
//	    sqlite3_transaction trans(this);
//		sqlite3 *_db;
		
};
}}}
#endif

