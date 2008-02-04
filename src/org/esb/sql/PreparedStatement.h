#ifndef PREPAREDSTATEMENT_H_
#define PREPAREDSTATEMENT_H_
#include <string>
//#include "ResultSet.h"
#include <sqlite3.h>
using namespace std;
namespace org{
	namespace esb{
		namespace sql{
			class ResultSet;
			class PreparedStatement{
				public:
					void setBlob(int pos, void*, long length);
					void setDouble(int pos, double val);
					void setInt(int pos, int val);
					void setNull(int pos);
					void setClob(int pos, void* val, long length);
					void setString(int pos, string val);
					bool execute();
					ResultSet & executeQuery();
					int executeUpdate();
				private:
					sqlite3_stmt *_pStmt;
			};
		}
	}
}
#endif /*PREPAREDSTATEMENT_H_*/
