#include "org/esb/io/File.h"
#include "org/esb/util/StringTokenizer.h"
#include "org/esb/util/Properties.h"
#include "Connection.h"

#include "Statement.h"
#include "PreparedStatement.h"
#include <iostream>
//#include "tntdb/connect.h"
using namespace org::esb::sql;
using namespace org::esb::io;
using namespace org::esb::util;

Connection::Connection() {

}
Connection::Connection(const char * con){
  std::string constr=con;
  parseConnectionString(constr);
  mysql_init(&mysql);
  if (!mysql_real_connect(&mysql,_host.c_str(),_username.c_str(),_passwd.c_str(),_db.c_str(),0,NULL,0))
  {
    throw SqlException(string("Failed to connect to database: Error:").append(mysql_error(&mysql)));
  }

}

Connection::Connection(const char * host, const char * db, const char * user, const char *pass){
  _username=user;
  _passwd=pass;
  _host=host;
  _db=db;
  mysql_init(&mysql);
  if (!mysql_real_connect(&mysql,_host.c_str(),_username.c_str(),_passwd.c_str(),_db.c_str(),0,NULL,0))
  {
    throw SqlException(string("Failed to connect to database: Error:").append(mysql_error(&mysql)));
  }
}

Connection::~Connection(){
  mysql_close(&mysql);
}


Statement Connection::createStatement(const char * sql){
	return Statement(mysql, sql);
}

PreparedStatement Connection::prepareStatement(const char * sql){
//	PreparedStatement *stmt=new PreparedStatement(*this, sql);
	return PreparedStatement(mysql, sql);
}

//Statement & Connection::createStatement(){return;}


void Connection::close(){
//	mysql_close(mysql);
//    sqlite3_connection::close();	
}
void Connection::executeNonQuery(const char * sql){
//    sqlite3_connection::close();	
}
long Connection::lastInsertId(){
  return mysql_insert_id(&mysql);
//	return tntcon.selectValue("select last_insert_id()").getInt();
//    sqlite3_connection::close();	
}
void Connection::parseConnectionString(std::string & constr){
    StringTokenizer tok(constr,":");
    if(tok.countTokens()==2){
        tok.nextToken();
        StringTokenizer data(tok.nextToken(),";");
        while(data.hasMoreTokens()){
            StringTokenizer line(data.nextToken(),"=");
            string key=line.nextToken();
            string val=line.nextToken();
            if(key.compare("user")==0){
                _username=val;
            }else 
            if(key.compare("passwd")==0){
              _passwd=val;
            }else
            if(key.compare("host")==0){
              _host=val;
            }else
            if(key.compare("db")==0){
              _db=val;
            }else{
              /*nothing*/
            }
        }
    }
}
