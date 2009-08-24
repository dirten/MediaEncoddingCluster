#include "org/esb/util/StringTokenizer.h"
#include "org/esb/util/Properties.h"
#include "Connection.h"
#include "org/esb/util/Log.h"
#include "Statement.h"
#include "PreparedStatement.h"
#include "org/esb/config/config.h"

using namespace org::esb::sql;
using namespace org::esb::util;
int Connection::_staticCounter = 0;

/*
Connection::Connection() {

}
 */
Connection::Connection(const char * con, bool auto_connect) {
  _constr = con;
  parseConnectionString(_constr);
  if (auto_connect)
    connect();
}

Connection::Connection(const std::string con, bool auto_connect) {
  _constr = con;
  parseConnectionString(con);
  if (auto_connect)
    connect();
}

Connection::Connection(std::string host, std::string db, std::string user, std::string pass, bool auto_connect) {
  _username = user;
  _passwd = pass;
  _host = host;
  _db = db;
  if (auto_connect)
    connect();
}

Connection::~Connection() {
  logdebug("Connection::~Connection()");
  //	mysql_thread_end();
  //mysql_close(mysqlPtr.get());
  /*
    _staticCounter--;
    if(_staticCounter==0){
      mysql_library_end();
    }
  if(mysqlPtr.use_count()==0)
    delete mysqlPtr.get();
   */
  //  std::cout <<"Connection Object destroyed"<<std::endl;
}

void Connection::connect() {
  logdebug("Connection::connect()");
  if (_staticCounter == 0) {
    std::string base_path = org::esb::config::Config::getProperty("hive.base_path");

    std::string lang = "--language=";
    lang.append(base_path);
    lang.append("/res");

    std::string datadir = "--datadir=";
    datadir.append(base_path);
#ifdef USE_EMBEDDED_MYSQL
    logdebug("Using Embedded Mysql");
#endif
    logdebug("DataDir:" << datadir);
    logdebug("LangDir:" << lang);
    static char *server_options[] = {"", const_cast<char*> (datadir.c_str()), const_cast<char*> (lang.c_str()), NULL};
    int num_elements = (sizeof (server_options) / sizeof (char *)) - 1;
    static char *server_groups[] = {"embedded", "server", NULL};
    mysql_library_init(num_elements, server_options, server_groups);
  }

  mysqlPtr = boost::shared_ptr<MYSQL > (mysql_init(NULL), &mysql_close);
  //  mysql_options(mysqlPtr.get(), MYSQL_OPT_USE_EMBEDDED_CONNECTION,NULL);
//  mysql_options(mysqlPtr.get(), MYSQL_OPT_USE_REMOTE_CONNECTION, NULL);
  unsigned int proto = MYSQL_PROTOCOL_TCP;
  mysql_options(mysqlPtr.get(), MYSQL_OPT_PROTOCOL, &proto);
  if (!mysql_real_connect(mysqlPtr.get(), _host.c_str(), _username.c_str(), _passwd.c_str(), _db.c_str(), 0, NULL, 0)) {
    throw SqlException(string("Failed to connect to database: ").append(std::string(mysql_error(mysqlPtr.get()))));
  }

  _staticCounter++;
}

Statement Connection::createStatement(const char * sql) {
  return Statement(mysqlPtr.get(), sql);
}

Statement * Connection::createStatement() {
  return new Statement(mysqlPtr.get(), "");
}

PreparedStatement Connection::prepareStatement(const char * sql) {
  return PreparedStatement(mysqlPtr.get(), sql);
}

PreparedStatement * Connection::prepareStatement2(const char * sql) {
  return new PreparedStatement(mysqlPtr.get(), sql);
}

void Connection::close() {

}

void Connection::executeNonQuery(std::string sql) {
  if (mysql_query(mysqlPtr.get(), sql.c_str()))
    throw SqlException(string("Failed to execute Query -> ").append(sql).append(" -> ").append(mysql_error(mysqlPtr.get())));
}

long long int Connection::lastInsertId() {
  return mysql_insert_id(mysqlPtr.get());
}

void Connection::parseConnectionString(std::string constr) {
  StringTokenizer tok(constr, ":");
  if (tok.countTokens() == 2) {
    tok.nextToken();
    StringTokenizer data(tok.nextToken(), ";");
    while (data.hasMoreTokens()) {
      StringTokenizer line(data.nextToken(), "=");
      string key = line.nextToken();
      string val = line.nextToken();
      //      logdebug("key:"<<key<<"="<<val);
      if (key.compare("user") == 0) {
        _username = val;
      } else
        if (key.compare("passwd") == 0) {
        _passwd = val;
      } else
        if (key.compare("host") == 0) {
        _host = val;
      } else
        if (key.compare("db") == 0) {
        _db = val;
      } else {
        /*nothing*/
      }
    }
  }
}
