#ifndef PREPAREDSTATEMENT_H_
#define PREPAREDSTATEMENT_H_
#include <string>
#include "Connection.h"
#include "ResultSet.h"

//#include "tntdb/statement.h"
//#include "ResultSet.h"
//#include "ResultSet.h"
//#include "sqlite3x.hpp"
#include "my_sql.h"
//#include "mysql++.h"
#include "Column.h"
#include "Parameter.cpp"

#include <boost/shared_ptr.hpp>
#include <boost/thread/shared_mutex.hpp>
using namespace std;
//using namespace sqlite3x;
namespace org {
  namespace esb {
    namespace sql {
      class Connection;
      class ResultSet;

      class PreparedStatement {
      public:
        PreparedStatement(MYSQL * mysql, const char * sql);
        //				    PreparedStatement(mysqlpp::Query stmt);
        ~PreparedStatement();
        void setDouble(string pos, double val);
        void setLong(string pos, long long int val);
        void setInt(string pos, int val);
        void setNull(string pos);
        void setBlob(string pos, char* data, int length);
        void setClob(string pos, char* data, int length);
        void setString(string pos, string val);
        void setString(string pos, char * val);
        unsigned long long getLastInsertId();
        int executeUpdate();
        bool execute();
        ResultSet executeQuery();
        ResultSet *executeQuery2();
        void close(void);
      private:
        Parameter * para;
        void parseSql(const char *);
        map<int, Column*> col_vars;
        map<std::string, int> vars;
        ResultSet * rs;
        //        MYSQL_STMT *stmt;
        boost::shared_ptr<MYSQL_STMT> stmtPtr;
//        boost::shared_ptr<boost::shared_mutex> mutexPtr;
        
        //				  char * psql;
        std::string sql;
        //					tntdb::Statement tntstmt;

      };
    }
  }
}
#endif /*PREPAREDSTATEMENT_H_*/
