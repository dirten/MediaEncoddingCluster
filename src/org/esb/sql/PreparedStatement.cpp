#include "PreparedStatement.h"
#include <iostream>
#include "tntdb/blob.h"
#include "Column.h"
using namespace org::esb::sql;

PreparedStatement::PreparedStatement(MYSQL & mysql, const char * sql):Statement(mysql, sql){
  para=new Parameter(stmt);
}


PreparedStatement::~PreparedStatement(){
  delete para;
}

void PreparedStatement::setBlob(string pos, char* data, int length){

	tntdb::Blob lob(data, length);
	tntstmt.setBlob(pos, lob);
}

void PreparedStatement::setClob(string pos, char* data, int length){
	tntdb::Blob lob(data, length);
	tntstmt.setBlob(pos, lob);
}

void PreparedStatement::setDouble(string pos, double data){
	tntstmt.setDouble(pos, data);
}

void PreparedStatement::setInt(string pos, int data){
	tntstmt.setInt(pos, data);
}

void PreparedStatement::setNull(string pos){
	tntstmt.setNull(pos);
}

void PreparedStatement::setString(string pos, string data){para->getParameter(vars[pos])->setString(data);}
void PreparedStatement::setString(string pos, char * data){para->getParameter(vars[pos])->setString(data);}


int PreparedStatement::executeUpdate(){

}

unsigned long long PreparedStatement::getLastInsertId(){

}
/*
const char * PreparedStatement::parseSql(const char * s){
    int parCount=0;
    std::string sqlIn=s;
    std::string sql;
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
      std::cout <<"SqlParsed"<<sql<<std::endl;
      psql=new char[sql.length()+1];
      memset(psql,0,sql.length()+1);
      memcpy(psql,sql.c_str(),sql.length());
      return psql;    
}
*/
