#ifndef ORG_ESB_SQL_CONNECTION_H
#define ORG_ESB_SQL_CONNECTION_H
#include <mysql/mysql.h>
#include "org/esb/util/Log.h"
#include "SqlException.h"
#include <string>
#include "tntdb/connection.h"
#include "mysql++.h"
namespace org {
    namespace esb {
        namespace sql {
            class PreparedStatement;
            class Statement;

            class Connection {
                logger("hive.sql.Connection")
            public:
                Connection() throw (SqlException);
                Connection(const char * con);
                Connection(const char * host,const char * db, const char * user, const char * pass);
                ~Connection();
                Statement createStatement(const char * sql);
                PreparedStatement prepareStatement(const char * sql);
                void executeNonQuery(const char * sql);
                long lastInsertId();
                //		sqlite3_transaction getTransaction();
                void close();

                static const int AUTOCOMMIT = 1;
                static const int USERCOMMIT = 2;
            private:
                friend class Statement;
                friend class PreparedStatement;
                tntdb::Connection tntcon;
                mysqlpp::Connection cppcon;
                
                std::string _username;
                std::string _passwd;
                std::string _host;
                std::string _port;
                std::string _db;
                
                void parseConnectionString(std::string &constr);
                MYSQL mysql;
            };
        }
    }
}
#endif

