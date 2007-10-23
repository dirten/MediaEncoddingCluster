#ifndef ORG_ESB_SQL_CONNECTION_H
#define ORG_ESB_SQL_CONNECTION_H
#include "org/esb/io/File.h"
#include "Statement.h"


using namespace org::esb::io;
namespace org{
namespace esb{
namespace sql{
class Connection {
	public:
		Connection(File & databaseFile);
		Statement & createStatement();
		void close();
};
}}}
#endif

