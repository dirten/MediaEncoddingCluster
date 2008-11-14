#include "Connection.h"
#include "Statement.h"
#include "ResultSet.h"

using namespace org::esb::sql;

Statement::Statement(MYSQL * mysql, const char * s) {
  logdebug("Statement::Statement(MYSQL * mysql, const char * s)");
//  stmt=mysql_stmt_init(mysql);
  stmtPtr=boost::shared_ptr<MYSQL_STMT>(mysql_stmt_init(mysql),&mysql_stmt_close);
  if (!stmtPtr.get()) {
    throw SqlException("mysql_stmt_init(), out of memory");
  }
  sql = s;
  if (mysql_stmt_prepare(stmtPtr.get(), sql.c_str(), strlen(sql.c_str()))) {
    throw SqlException(string("failed while prepare the statement: ").append(mysql_stmt_error(stmtPtr.get())).append(" " + sql));
  }
}

Statement::~Statement() {
  logdebug("Statement::~Statement()");
//  close();
}

ResultSet Statement::executeQuery() {
  execute();
  return ResultSet(*stmtPtr.get());
}

ResultSet Statement::executeQuery(char* tmp) {
  throw SqlException(" Statement::executeQuery(char* tmp) !! not implemented!");
  return ResultSet(*stmtPtr.get());
}

bool Statement::execute() {
  if (mysql_stmt_execute(stmtPtr.get())) {
    throw SqlException(string("failed while execute the statement: ").append(mysql_stmt_error(stmtPtr.get())));
  }
  return true;
}

void Statement::close() {
  if (mysql_stmt_free_result(stmtPtr.get())) {
    throw SqlException(string("failed while freeing the statement: ").append(mysql_stmt_error(stmtPtr.get())));
  }
  if (mysql_stmt_close(stmtPtr.get())) {
    throw SqlException(string("failed while closing the statement: ").append(mysql_stmt_error(stmtPtr.get())));
  }
}

