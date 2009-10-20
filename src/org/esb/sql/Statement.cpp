#include "Connection.h"
#include "Statement.h"
#include "ResultSet.h"
#include "org/esb/util/Log.h"

using namespace org::esb::sql;

Statement::Statement(MYSQL * mysql, const char * s) : rs(NULL) {
//  stmt=mysql_stmt_init(mysql);
  rs = NULL;
  stmtPtr = boost::shared_ptr<MYSQL_STMT > (mysql_stmt_init(mysql),&mysql_stmt_close);
  if (!stmtPtr.get()) {
    throw SqlException(std::string("mysql_stmt_init(), out of memory"));
  }
  sql = s;
}

Statement::~Statement() {
//    logdebug("Statement::~Statement()");
  //  delete rs;
//    close();
}

ResultSet * Statement::executeQuery(std::string s) {
  sql = s;
  execute();
  if (!rs)
    rs = new ResultSet(stmtPtr.get());
  return rs;
}

ResultSet * Statement::executeQuery2() {
  execute();
  if (!rs)
    rs = new ResultSet(stmtPtr.get());
  return rs;
}

ResultSet Statement::executeQuery() {
  execute();
    rs = new ResultSet(stmtPtr.get());
  return *rs;
}

/*
ResultSet Statement::executeQuery(char* tmp) {
        throw SqlException(std::string(" Statement::executeQuery(char* tmp) !! not implemented!"));
  return ResultSet(*stmtPtr.get());
}
 */
bool Statement::execute() {
  if (mysql_stmt_prepare(stmtPtr.get(), sql.c_str(), sql.length())) {
    throw SqlException(string("failed while prepare the statement: ").append(mysql_stmt_error(stmtPtr.get())).append(" " + sql));
  }
  if (mysql_stmt_execute(stmtPtr.get())) {
    throw SqlException(std::string("failed while execute the statement: ").append(mysql_stmt_error(stmtPtr.get())));
  }
  return true;
}

void Statement::close() {

  if (mysql_stmt_free_result(stmtPtr.get())) {
    throw SqlException(std::string("failed while freeing the statement: ").append(mysql_stmt_error(stmtPtr.get())));
  }
  if (mysql_stmt_close(stmtPtr.get())) {
    throw SqlException(std::string("failed while closing the statement: ").append(mysql_stmt_error(stmtPtr.get())));
  }

}

