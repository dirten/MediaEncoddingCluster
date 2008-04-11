#ifndef ORG_ESB_SQL_RESULTSET_H
#define ORG_ESB_SQL_RESULTSET_H
#include "Statement.h"
#include "PreparedStatement.h"
#include "org/esb/util/Log.h"
//#include "sqlite3x.hpp"
#include <mysql/mysql.h>
#include "tntdb/result.h"
#include "tntdb/statement.h"
#include "tntdb/row.h"
#include <vector>
//using namespace sqlite3x;
namespace org{
namespace esb{
namespace sql{
class Column;
class ResultSet{
	public:
	bool 		next();
	
    bool 		getBool			(int index);
    bool 		getBool			(string index);
    int 		getInt			(int index);
    int 		getInt			(string index);
    double 		getDouble		(int index);
    double 		getDouble		(string index);

	string 		getString		(int col);
	string 		getString		(string col);

	string 		getBlob			(int index);
	string 		getBlob			(string index);

	string 		getClob			(int index);
	string 		getClob			(string index);

	float 		getFloat		(int col);
	float 		getFloat		(string col);

	bool 		isNull			(int col);
	bool 		isNull			(string col);

	private:
		friend class Statement;
		friend class PreparedStatement;
//		ResultSet(MYSQL_STMT * stmt);
		bool isBeforeFirst;
		ResultSet(tntdb::Result result);
		ResultSet(tntdb::Statement result);
		tntdb::Result tntresult;
		tntdb::Statement tntstmt;
		tntdb::Row tntrow;
		tntdb::Statement::const_iterator tntiterator;

};
}}}
#endif

