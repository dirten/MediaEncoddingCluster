#ifndef ORG_ESB_SQL_RESULTSET_H
#define ORG_ESB_SQL_RESULTSET_H
#include "Statement.h"
#include "sqlite3x.hpp"
using namespace sqlite3x;
namespace org{
namespace esb{
namespace sql{

class ResultSet:public sqlite3_reader{
	public:
		ResultSet(Statement & stmt);
		bool next();

};
}}}
#endif

