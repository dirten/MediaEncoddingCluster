#ifndef ORG_ESB_SQL_RESULTSET_H
#define ORG_ESB_SQL_RESULTSET_H
#include "Statement.h"
#include "PreparedStatement.h"
//#include "sqlite3x.hpp"
#include <mysql/mysql.h>
//#include "boost/shared_ptr.hpp"
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
    unsigned 	getUnsigned		(int index);
    unsigned 	getUnsigned		(string index);
    long 		getLong			(int index);
    long 		getLong			(string index);
    int32_t 	getInt32		(int index);
    int32_t 	getInt32		(string index);
    uint32_t 	getUnsigned32	(int index);
    uint32_t 	getUnsigned32	(string index);
    int64_t 	getInt64		(int index);
    int64_t 	getInt64		(string index);
    uint64_t	getUnsigned64	(int index);
    uint64_t	getUnsigned64	(string index);

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
		ResultSet(MYSQL_STMT & stmt);
		MYSQL_STMT & _stmt;
		MYSQL_RES* _resultSetMetadata;
		MYSQL_ROW _record;
		MYSQL_BIND * _bindColumns;
//		unsigned long * _lengths;
		int getColumnIndex(string name);
//		char data[255];
//		my_bool isNullB;
//		my_bool isError;
//		unsigned long length;
		vector<Column*> _row;
		template <typename int_type> 
			int_type getInteger(const MYSQL_BIND& bind);
    	template <typename float_type>
    		float_type getFloat(const MYSQL_BIND& bind);

};
}}}
#endif

