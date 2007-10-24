#ifndef PREPAREDSTATEMENT_H_
#define PREPAREDSTATEMENT_H_
#include <string>
#include "ResultSet.h"
using namespace std;
namespace org{
	namespace esb{
		namespace sql{
			class PreparedStatement{
				public:
					void setBlob(int pos, void*);
					void setDouble(int pos, double val);
					void setInt(int pos, int val);
					void setNull(int pos);
					void setClob(int pos, void* val);
					void setString(int pos, string val);
					bool execute();
					ResultSet & executeQuery();
					int executeUpdate();
			};
		}
	}
}
#endif /*PREPAREDSTATEMENT_H_*/
