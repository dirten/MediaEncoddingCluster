#ifndef ORG_ESB_SQL_STATEMENT_H
#define ORG_ESB_SQL_STATEMENT_H
#include <string>
#include <map>
//#include "Connection.h"

#include "my_sql.h"
#include "SqlException.h"

#include <boost/shared_ptr.hpp>

//using namespace std;
namespace org {
    namespace esb {
        namespace sql {
            //      class Connection;
            class ResultSet;

            class Statement {
              classlogger("org.esb.sql.Statement")
            public:
                Statement(MYSQL * mysql, const char * sql);
                ~Statement();
                ResultSet executeQuery();
                ResultSet * executeQuery(std::string sql);
                ResultSet * executeQuery2();
                //        ResultSet executeQuery(char *);
                bool execute();
                void close();
            protected:
                friend class ResultSet;
//                MYSQL_STMT * stmt;
                boost::shared_ptr<MYSQL_STMT> stmtPtr;
//                std::map<std::string, int> vars;
            private:
                std::string sql;
                ResultSet * rs;

            };
        }
    }
}
#endif


