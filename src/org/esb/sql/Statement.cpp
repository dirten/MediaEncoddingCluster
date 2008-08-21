#include "Connection.h"
#include "Statement.h"
#include "ResultSet.h"

using namespace org::esb::sql;

Statement::Statement(MYSQL & mysql, const char * s):tntstmt(NULL),cppstmt(NULL){
  stmt = mysql_stmt_init(&mysql);
  if (!stmt){
    throw SqlException( "mysql_stmt_init(), out of memory");
  }
  parseSql(s);
  if (mysql_stmt_prepare(stmt, sql.c_str(), strlen(sql.c_str()))){
    throw SqlException( mysql_stmt_error(stmt));
  }
}

Statement::~Statement(){
	close();
}

ResultSet Statement::executeQuery(){
	return ResultSet(*this);
}


ResultSet Statement::executeQuery(char* tmp){

}

bool Statement::execute(){
	return cppstmt.execute()==0;
}

void Statement::close(){
  if (mysql_stmt_close(stmt)){
    throw SqlException( string("failed while closing the statement: ").append( mysql_stmt_error(stmt)));
  }
}

void Statement::parseSql(const char * s){
    int parCount=0;
    std::string sqlIn=s;
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

      logdebug("parse sql \"" << sqlIn << "\"");

      for (std::string::const_iterator it = sqlIn.begin();
           it != sqlIn.end(); ++it)
      {
        char ch = *it;
        switch(state)
        {
          case STATE_0:
            if (ch == ':')
              state = STATE_NAME0;
            else if (ch == '\\')
              state = STATE_ESC;
            else
            {
              sql += ch;
              if (ch == '\'' || ch == '"' || ch == '`')
              {
                state = STATE_STRING;
                end_token = ch;
              }
            }
            break;

          case STATE_NAME0:
            if (std::isalpha(ch))
            {
              name = ch;
              state = STATE_NAME;
            }
            else if (ch == ':')
              sql += ':';
            else if (ch == '\\')
            {
              sql += ':';
              state = STATE_ESC;
            }
            break;

          case STATE_NAME:
            if (std::isalnum(ch) || ch == '_')
              name += ch;
            else
            {
              logdebug("hostvar :" << name);
              vars[name]=parCount++;
              sql += '?';//event.onHostVar(name);

//              sql += event.onHostVar(name);
              if (ch == '\\')
                state = STATE_ESC;
              else
              {
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

      switch(state)
      {
        case STATE_NAME0:
          sql += ':';
          break;

        case STATE_NAME:
          logdebug("hostvar :" << name);
          vars[name]=parCount++;
          sql += '?';//event.onHostVar(name);
          break;

        default:
          ;
      }
      cout<<"ParseSql" << sql<<endl;
}

