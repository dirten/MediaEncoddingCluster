#ifndef PREPAREDSTATEMENT_H_
#define PREPAREDSTATEMENT_H_
#include <string>
#include "Connection.h"
#include "ResultSet.h"
#include "org/esb/util/Log.h"
//#include "ResultSet.h"
//#include "ResultSet.h"
//#include "sqlite3x.hpp"
#include "mysql/mysql.h"
using namespace std;
//using namespace sqlite3x;
namespace org{
	namespace esb{
		namespace sql{
			class Connection;
			class ResultSet;
			class PreparedStatement{
			    logger("hive.sql")
				public:
				    PreparedStatement(Connection & db, const char * sql);
				    ~PreparedStatement();
					void setDouble(int pos, double val);
					void setInt(int pos, int val);
					void setNull(int pos);
					void setBlob(int pos, char* data, int length);
					void setClob(int pos, char* data, int length);
					void setString(int pos, string & val);
					void setString(int pos, char * val);
					bool execute();
					unsigned long long getLastInsertId();
					int executeUpdate();
					ResultSet executeQuery();
				private:
					Connection _con;
					const char * _sql;
					MYSQL_STMT* _stmt;
					MYSQL_BIND * _col;
					my_bool * _is_error;
					my_bool * _is_null;
//					int * _data_length;
					unsigned long _columnCount;
					void bind(int col, enum_field_types buffer_type,char * data, int data_length);
					template <typename c_type>
						void setValue(int col, c_type value, enum_field_types mysql_type);


			};
		}
	}
}
#endif /*PREPAREDSTATEMENT_H_*/
