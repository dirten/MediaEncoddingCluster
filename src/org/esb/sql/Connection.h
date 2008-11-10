#ifndef ORG_ESB_SQL_CONNECTION_H
#define ORG_ESB_SQL_CONNECTION_H
#include "org/esb/util/Log.h"
#include "SqlException.h"
#include <string>
#include "my_sql.h"

//#include "tntdb/connection.h"
//#include "mysql++.h"
namespace org {
    namespace esb {
        namespace sql {
            class PreparedStatement;
            class Statement;

            class Connection {
                logger("hive.sql.Connection")
            public:
                Connection();
                Connection(const char * con);
                Connection(std::string host,std::string db, std::string user, std::string pass);
                ~Connection();
                Statement createStatement(const char * sql);
                PreparedStatement prepareStatement(const char * sql);
                void executeNonQuery(std::string sql);
                long lastInsertId();
                //		sqlite3_transaction getTransaction();
                void close();

                static const int AUTOCOMMIT = 1;
                static const int USERCOMMIT = 2;
            private:
                friend class Statement;
                friend class PreparedStatement;
                std::string _username;
                std::string _passwd;
                std::string _host;
                std::string _port;
                std::string _db;
                MYSQL mysql;
            private:
                friend class Setup;
                void parseConnectionString(std::string &constr);
            };
        }
    }
}
#endif

