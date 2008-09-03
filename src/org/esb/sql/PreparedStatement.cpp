#include "PreparedStatement.h"
#include <iostream>
//#include "tntdb/blob.h"
#include "Column.h"
using namespace org::esb::sql;

PreparedStatement::PreparedStatement(MYSQL & mysql, const char * s){

  stmt = mysql_stmt_init(&mysql);
  if (!stmt){
    throw SqlException( "mysql_stmt_init(), out of memory");
  }
  parseSql(s);
  if (mysql_stmt_prepare(stmt, sql.c_str(), strlen(sql.c_str()))){
    throw SqlException( string("failed while prepare the statement: ").append(mysql_stmt_error(stmt)).append(sql));
//    throw SqlException( mysql_stmt_error(stmt));
  }
  para=new Parameter(stmt);
}


PreparedStatement::~PreparedStatement(){
  delete para;
  close();
}
void PreparedStatement::close(){
  if (mysql_stmt_close(stmt)){
    throw SqlException( string("failed while closing the statement: ").append( mysql_stmt_error(stmt)));
  }
}

void PreparedStatement::setBlob(string pos, char* data, int length){para->getParameter(vars[pos])->setBlob(data);}
void PreparedStatement::setClob(string pos, char* data, int length){para->getParameter(vars[pos])->setString(data);}


void PreparedStatement::setDouble(string pos, double data){para->getParameter(vars[pos])->setDouble(data);}

void PreparedStatement::setInt(string pos, int data){para->getParameter(vars[pos])->setInt(data);}

void PreparedStatement::setNull(string pos){para->getParameter(vars[pos])->setNull();}

void PreparedStatement::setString(string pos, string data){para->getParameter(vars[pos])->setString(data);}
void PreparedStatement::setString(string pos, char * data){para->getParameter(vars[pos])->setString(data);}


ResultSet PreparedStatement::executeQuery(){
/*
    if (mysql_stmt_bind_param(stmt, para->bind)){
      throw SqlException( mysql_stmt_error(stmt));
    }
*/
    execute();
	return ResultSet(*stmt);
}

int PreparedStatement::executeUpdate(){

}

bool PreparedStatement::execute(){

  if (mysql_stmt_bind_param(stmt, para->bind)){

    throw SqlException( string("failed while bind the param: ").append(mysql_stmt_error(stmt)));

//    throw SqlException( mysql_stmt_error(stmt));
  }

  if (mysql_stmt_execute(stmt)){
    throw SqlException( string("failed while execute the statement: ").append(mysql_stmt_error(stmt)));
//    throw SqlException( mysql_stmt_error(stmt));
  }
  return true;
}

unsigned long long PreparedStatement::getLastInsertId(){

}

void PreparedStatement::parseSql(const char * s){
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

//      logdebug("parse sql \"" << sqlIn << "\"");

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
//              logdebug("hostvar :" << name);
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
//          logdebug("hostvar :" << name);
          vars[name]=parCount++;
          sql += '?';//event.onHostVar(name);
          break;

        default:
          ;
      }
//      cout<<"ParseSql" << sql<<endl;
}