#include "org/esb/util/StringTokenizer.h"
#include "org/esb/util/Properties.h"
#include "Connection.h"

#include "Statement.h"
#include "PreparedStatement.h"


using namespace org::esb::sql;
using namespace org::esb::util;

Connection::Connection() {

}

Connection::Connection(const char * con) {
  std::string constr = con;
  parseConnectionString(constr);
  mysql_init(&mysql);
  if (!mysql_real_connect(&mysql, _host.c_str(), _username.c_str(), _passwd.c_str(), _db.c_str(), 0, NULL, 0)) {
    throw SqlException(string("Failed to connect to database: Error:").append(mysql_error(&mysql)));
  }
  mysql_set_server_option(&mysql, MYSQL_OPTION_MULTI_STATEMENTS_ON);
}

Connection::Connection(std::string host, std::string db, std::string user, std::string pass) {
  _username = user;
  _passwd = pass;
  _host = host;
  _db = db;
  mysql_init(&mysql);
  if (!mysql_real_connect(&mysql, _host.c_str(), _username.c_str(), _passwd.c_str(), _db.c_str(), 0, NULL, 0)) {
    throw SqlException(string("Failed to connect to database: Error:").append(mysql_error(&mysql)));
  }
  mysql_set_server_option(&mysql, MYSQL_OPTION_MULTI_STATEMENTS_ON);
}

Connection::~Connection() {
  mysql_close(&mysql);
}

Statement Connection::createStatement(const char * sql) {
  return Statement(mysql, sql);
}

PreparedStatement Connection::prepareStatement(const char * sql) {
  return PreparedStatement(mysql, sql);
}

void Connection::close() {

}

void Connection::executeNonQuery(std::string sql) {
  if(mysql_query(&mysql,sql.c_str()))
    throw SqlException(string("Failed to execute Query ->\n").append(sql).append("->\n").append(mysql_error(&mysql)));
}

long Connection::lastInsertId() {
  return mysql_insert_id(&mysql);
}

void Connection::parseConnectionString(std::string & constr) {
  StringTokenizer tok(constr, ":");
  if (tok.countTokens() == 2) {
    tok.nextToken();
    StringTokenizer data(tok.nextToken(), ";");
    while (data.hasMoreTokens()) {
      StringTokenizer line(data.nextToken(), "=");
      string key = line.nextToken();
      string val = line.nextToken();
      if (key.compare("user") == 0) {
        _username = val;
      }else
        if (key.compare("passwd") == 0) {
        _passwd = val;
      }else
        if (key.compare("host") == 0) {
        _host = val;
      }else
        if (key.compare("db") == 0) {
        _db = val;
      }else {
        /*nothing*/
      }
    }
  }
}
