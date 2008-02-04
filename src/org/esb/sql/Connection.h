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
		Statement & createStatement(const char * sql);
		Statement & createStatement();
		PreparedStatement & prepareStatement(const char * sql);
		void close();
	private:
//		sqlite3 *_db;
		
};
}}}
#endif

