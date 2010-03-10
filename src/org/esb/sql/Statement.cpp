#include "Connection.h"
#include "Statement.h"
#include "ResultSet.h"
#include "org/esb/util/Log.h"
#include "org/esb/util/StringUtil.h"
using namespace org::esb::sql;

Statement::Statement(MYSQL * mysql, const char * s) : rs(NULL) {
  //  stmt=mysql_stmt_init(mysql);
  rs = NULL;
  stmtPtr = boost::shared_ptr<MYSQL_STMT > (mysql_stmt_init(mysql), &mysql_stmt_close);
  //  stmtPtr = boost::shared_ptr<MYSQL_STMT > (mysql_stmt_init(mysql),boost::bind(&Statement::close, this));
#ifndef __WIN32__
  if(StringUtil::toLower(sql).find("select")!=string::npos)
    free(stmtPtr.get()->mem_root.free);
#endif
  if (!stmtPtr.get()) {
    throw SqlException(std::string("mysql_stmt_init(), out of memory"));
  }
  sql = s;
  LOGTRACE("Statement::Statement()" << this);
}

Statement::~Statement() {
  LOGTRACE( "Statement::~Statement()" << this);
  //  delete rs;
  close();
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

/**
 * @TODO: this is a memory leak and a performance issue of Object copy (aaaaargh)
 */
ResultSet Statement::executeQuery() {
  execute();
  /*  if (rs)
      delete rs;
      rs = new ResultSet(stmtPtr.get());*/
  return ResultSet(stmtPtr.get());
}

/*
ResultSet Statement::executeQuery(char* tmp) {
        throw SqlException(std::string(" Statement::executeQuery(char* tmp) !! not implemented!"));
  return ResultSet(*stmtPtr.get());
}
 */
bool Statement::execute() {
  //  mysql_thread_init();
  if (mysql_stmt_prepare(stmtPtr.get(), sql.c_str(), sql.length())) {
    throw SqlException(string("failed while prepare the statement: ").append(mysql_stmt_error(stmtPtr.get())).append(" " + sql));
  }
  if (mysql_stmt_execute(stmtPtr.get())) {
    throw SqlException(std::string("failed while execute the statement: ").append(mysql_stmt_error(stmtPtr.get())));
  }
  //  mysql_thread_end();
  return true;
}

void Statement::close() {
  LOGTRACE( "freeing result set");
  if (mysql_stmt_free_result(stmtPtr.get())) {
    throw SqlException(std::string("failed while freeing the statement: ").append(mysql_stmt_error(stmtPtr.get())));
  }
  /*
  if (mysql_stmt_close(stmtPtr.get())) {
    throw SqlException(std::string("failed while closing the statement: ").append(mysql_stmt_error(stmtPtr.get())));
  }*/

}

