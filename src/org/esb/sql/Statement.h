#ifndef ORG_ESB_SQL_STATEMENT_H
#define ORG_ESB_SQL_STATEMENT_H
#include <string>
#include "ResultSet.h"

using namespace std;

namespace org{
namespace esb{
namespace sql{

class Statement {
	public:
		Statement();
		ResultSet & executeQuery (string sql);

};
}}}
#endif


