#include "org/esb/util/StringTokenizer.h"
#include "org/esb/util/Properties.h"
#include "Connection.h"

#include "Statement.h"
#include "PreparedStatement.h"


using namespace org::esb::sql;
using namespace org::esb::util;
int Connection::_staticCounter=0;
/*
Connection::Connection() {

}
*/
Connection::Connection(const char * con) {
  std::string constr = con;
  parseConnectionString(constr);
  if(_staticCounter==0){
    static char *server_options[] =  { "", "--datadir=.","--language=/usr/local/mysql-5.1.29/share/mysql/english", NULL };
    int num_elements = (sizeof(server_options) / sizeof(char *)) - 1;
    static char *server_groups[] = { "embedded","server", NULL };
    mysql_library_init(num_elements, server_options, server_groups);
  }
  _staticCounter++;
//   mysql=mysql_init(NULL);
    mysqlPtr=boost::shared_ptr<MYSQL>(mysql_init(NULL),&mysql_close);
  
  if (!mysql_real_connect(mysqlPtr.get(), _host.c_str(), _username.c_str(), _passwd.c_str(), _db.c_str(), 0, NULL, 0)) {
    throw SqlException(string("Failed to connect to database: ").append(mysql_error(mysqlPtr.get())).append(con));
  }
//  mysql_set_server_option(&mysql, MYSQL_OPTION_MULTI_STATEMENTS_ON);
}

Connection::Connection(std::string host, std::string db, std::string user, std::string pass) {
  _username = user;
  _passwd = pass;
  _host = host;
  _db = db;
  if(_staticCounter==0){
    static char *server_options[] =  { "", "--datadir=.","--language=/usr/local/mysql-5.1.29/share/mysql/english", NULL };
    int num_elements = (sizeof(server_options) / sizeof(char *)) - 1;
    static char *server_groups[] = { "embedded","server", NULL };
    mysql_library_init(num_elements, server_options, server_groups);
  }
  _staticCounter++;

//  mysql=mysql_init(NULL);
    mysqlPtr=boost::shared_ptr<MYSQL>(mysql_init(NULL),&mysql_close);
  if (!mysql_real_connect(mysqlPtr.get(), _host.c_str(), _username.c_str(), _passwd.c_str(), _db.c_str(), 0, NULL, 0)) {
    throw SqlException(string("Failed to connect to database: Error:").append(mysql_error(mysqlPtr.get())));
  }
//  mysql_set_server_option(&mysql, MYSQL_OPTION_MULTI_STATEMENTS_ON);
}

Connection::~Connection() {
  //mysql_close(mysqlPtr.get());
/*
  _staticCounter--;
  if(_staticCounter==0){
    mysql_library_end();
  }
*/
}

Statement Connection::createStatement(const char * sql) {
  return Statement(mysqlPtr.get(), sql);
}

PreparedStatement Connection::prepareStatement(const char * sql) {
  return PreparedStatement(*mysqlPtr.get(), sql);
}

void Connection::close() {

}

void Connection::executeNonQuery(std::string sql) {
  if(mysql_query(mysqlPtr.get(),sql.c_str()))
    throw SqlException(string("Failed to execute Query -> ").append(sql).append(" -> ").append(mysql_error(mysqlPtr.get())));
}

long Connection::lastInsertId() {
  return mysql_insert_id(mysqlPtr.get());
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
