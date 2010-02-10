#include "PreparedStatement.h"
#include <iostream>
//#include "tntdb/blob.h"
#include "Column.h"
#include "org/esb/util/Log.h"
using namespace org::esb::sql;

PreparedStatement::PreparedStatement(MYSQL * mysql, const char * s) {
	LOGINFO("SQL:"<<s);
//  logdebug("PreparedStatement::PreparedStatement(MYSQL & mysql, const char * s)"<<this);
  stmtPtr = boost::shared_ptr<MYSQL_STMT>(mysql_stmt_init(mysql),&mysql_stmt_close);
  if (!stmtPtr.get()) {
	  throw SqlException(std::string("mysql_stmt_init(), out of memory"));
  }
  parseSql(s);
  
  if (mysql_stmt_prepare(stmtPtr.get(), sql.c_str(), sql.length())) {
    throw SqlException(string("failed while prepare the statement: ").append(mysql_stmt_error(stmtPtr.get())).append(sql));
    //    throw SqlException( mysql_stmt_error(stmt));
  }
  para = new Parameter(stmtPtr.get());
  rs=NULL;
//  mutexPtr=boost::shared_ptr<boost::shared_mutex>(new boost::shared_mutex());
//  execute_mutex=;
}

PreparedStatement::~PreparedStatement() {
//  logdebug("PreparedStatement::~PreparedStatement()"<<this);
  delete para;
  delete rs;
//  close();
}

void PreparedStatement::close() {
  if (mysql_stmt_close(stmtPtr.get())) {
    throw SqlException(string("failed while closing the statement: ").append(mysql_stmt_error(stmtPtr.get())));
  }
}

void PreparedStatement::setBlob(string pos, char* data, int length) {
  para->getParameter(vars[pos])->setBlob(data, length);
}

void PreparedStatement::setClob(string pos, char* data, int length) {
  para->getParameter(vars[pos])->setString(data);
}

void PreparedStatement::setDouble(string pos, double data) {
  para->getParameter(vars[pos])->setDouble(data);
}
void PreparedStatement::setLong(string pos, long long int data) {
  para->getParameter(vars[pos])->setLong(data);
}

void PreparedStatement::setInt(string pos, int data) {
  para->getParameter(vars[pos])->setInt(data);
}

void PreparedStatement::setNull(string pos) {
  para->getParameter(vars[pos])->setNull();
}

void PreparedStatement::setString(string pos, string data) {
  para->getParameter(vars[pos])->setString(data);
}

void PreparedStatement::setString(string pos, char * data) {
  para->getParameter(vars[pos])->setString(data);
}

ResultSet PreparedStatement::executeQuery() {
  /*
      if (mysql_stmt_bind_param(stmt, para->bind)){
        throw SqlException( mysql_stmt_error(stmt));
      }
   */
  execute();
  if(!rs){
    rs=new ResultSet(stmtPtr.get());
  }
  return *rs;
}
ResultSet * PreparedStatement::executeQuery2() {
  /*
      if (mysql_stmt_bind_param(stmt, para->bind)){
        throw SqlException( mysql_stmt_error(stmt));
      }
   */
  execute();
  if(!rs){
    rs=new ResultSet(stmtPtr.get());
  }
  return rs;
}

int PreparedStatement::executeUpdate() {
  return 0;
}

bool PreparedStatement::execute() {
//  boost::mutex::scoped_lock scoped_lock((*mutexPtr.get()));
//  mysql_thread_init();
  if(mysql_stmt_param_count(stmtPtr.get())>0){
    if (mysql_stmt_bind_param(stmtPtr.get(), para->bind)) {

      throw SqlException(string("failed while bind the param: ").append(mysql_stmt_error(stmtPtr.get())));

      //    throw SqlException( mysql_stmt_error(stmt));
    }
  }

  if (mysql_stmt_execute(stmtPtr.get())) {
    throw SqlException(string("failed while execute the statement: ").append(mysql_stmt_error(stmtPtr.get())));
    //    throw SqlException( mysql_stmt_error(stmt));
  }
//  mysql_thread_end();
  return true;
}

unsigned long long PreparedStatement::getLastInsertId() {
  return mysql_stmt_insert_id(stmtPtr.get());
}

void PreparedStatement::parseSql(const char * s) {
  int parCount = 0;
  std::string sqlIn = s;

  enum state_type {
    STATE_0,
    STATE_NAME0,
    STATE_NAME,
    STATE_STRING,
    STATE_STRING_ESC,
    STATE_ESC
  } state = STATE_0;

  std::string name;

  sql.clear();
  char end_token;

  //      logdebug("parse sql \"" << sqlIn << "\"");

  for (std::string::const_iterator it = sqlIn.begin();
      it != sqlIn.end(); ++it) {
    char ch = *it;
    switch (state) {
      case STATE_0:
        if (ch == ':')
          state = STATE_NAME0;
        else if (ch == '\\')
          state = STATE_ESC;
        else {
          sql += ch;
          if (ch == '\'' || ch == '"' || ch == '`') {
            state = STATE_STRING;
            end_token = ch;
          }
        }
        break;

      case STATE_NAME0:
        if (isalpha(ch)) {
          name = ch;
          state = STATE_NAME;
        } else if (ch == ':')
          sql += ':';
        else if (ch == '\\') {
          sql += ':';
          state = STATE_ESC;
        }
        break;

      case STATE_NAME:
        if (isalnum(ch) || ch == '_')
          name += ch;
        else {
          //              logdebug("hostvar :" << name);
          vars[name] = parCount++;
          sql += '?'; //event.onHostVar(name);

          //              sql += event.onHostVar(name);
          if (ch == '\\')
            state = STATE_ESC;
          else {
            sql += ch;
            state = STATE_0;
          }
        }
        break;

      case STATE_STRING:
        sql += ch;
        if (ch == end_token)
          state = STATE_0;
        else if (ch == '\\')
          state = STATE_STRING_ESC;
        break;

      case STATE_STRING_ESC:
        sql += ch;
        state = STATE_STRING;
        break;

      case STATE_ESC:
        sql += ch;
        state = STATE_0;
        break;
    }
  }

  switch (state) {
    case STATE_NAME0:
      sql += ':';
      break;

    case STATE_NAME:
      //          logdebug("hostvar :" << name);
      vars[name] = parCount++;
      sql += '?'; //event.onHostVar(name);
      break;

    default:
      ;
  }
  //      cout<<"ParseSql" << sql<<endl;
}

