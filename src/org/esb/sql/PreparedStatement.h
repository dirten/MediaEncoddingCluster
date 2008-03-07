#ifndef PREPAREDSTATEMENT_H_
#define PREPAREDSTATEMENT_H_
#include <string>
//#include "ResultSet.h"
#include "sqlite3x.hpp"
using namespace std;
using namespace sqlite3x;
namespace org{
	namespace esb{
		namespace sql{
			class Connection;
			class ResultSet;
			class PreparedStatement:public sqlite3_command {
				public:
				    PreparedStatement(Connection & db, const char * sql);
					void setDouble(int pos, double val);
					void setInt(int pos, int val);
					void setNull(int pos);
					void setBlob(int pos, void*, int length);
					void setClob(int pos, void* val, int length);
					void setString(int pos, string val);
					bool execute();
//					ResultSet & executeQuery();
					int executeUpdate();
				private:
					sqlite3_stmt *_pStmt;
//					void bind(int pos, void* val);

			};
		}
	}
}
#endif /*PREPAREDSTATEMENT_H_*/
