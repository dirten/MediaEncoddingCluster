#include "Connection.h"
#include "Statement.h"
#include "ResultSet.h"

using namespace org::esb::sql;

Statement::Statement(MYSQL & mysql, const char * sql):tntstmt(NULL),cppstmt(NULL){
  stmt = mysql_stmt_init(&mysql);
  if (!stmt){
    throw SqlException( "mysql_stmt_init(), out of memory");
  }
  if (mysql_stmt_prepare(stmt, sql, strlen(sql))){
    throw SqlException( mysql_stmt_error(stmt));
  }
}



Statement::Statement(mysqlpp::Query & stmt):cppstmt(stmt){
  std::cerr << "Statement Error"<<cppstmt.error()<<std::endl;
  std::cerr << "Statement"<<cppstmt.str()<<std::endl;
}
/*
Statement::Statement(mysqlpp::Query *stmt):cppstmt(*stmt){
  std::cerr << "Statement Error"<<cppstmt.error()<<std::endl;
  std::cerr << "Statement"<<cppstmt.str()<<std::endl;
}
*/
Statement::Statement(mysqlpp::Connection &con, const char * sql):cppstmt(NULL), _con(con){
//  mysqlpp::Query temp(&con, true, sql);
  std::cerr << "Connected"<<con.connected()<<std::endl;
  std::cerr << "ConnectionServerStatus "<<con.server_status()<<std::endl;

  query=new mysqlpp::Query(&con, false, sql);
  std::cerr << "Statement Error"<<query->error()<<std::endl;
  std::cerr << "Statement"<<query->str()<<std::endl;
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

