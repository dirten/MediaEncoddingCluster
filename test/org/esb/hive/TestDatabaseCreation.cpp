#include "org/esb/sql/Connection.h"
#include "org/esb/io/File.h"
#include "org/esb/io/FileInputStream.h"
#include "org/esb/util/StringTokenizer.h"
#include "org/esb/util/StringUtil.h"
#include "org/esb/config/config.h"
#include "org/esb/hive/DatabaseService.h"
using namespace org::esb;

int main() {
  std::string src = MEC_SOURCE_DIR;
  std::string sqlfile = src;
  sqlfile.append("/sql/hive-0.0.3.sql");
  LOGDEBUG("loading DB model from " << sqlfile);
  std::string sql;
  io::File f(sqlfile.c_str());
  io::FileInputStream(&f).read(sql);
//  logdebug("FullSql:" << sql);
  config::Config::setProperty("hive.base_path", src.c_str());
  config::Config::setProperty("db.connection", "mysql:host=localhost;db=hive;user=;passwd=");
  hive::DatabaseService::start(src);
  //config::Config::setProperty("db.connection","")
  {
    sql::Connection con_a("");
    con_a.executeNonQuery(string("CREATE DATABASE hive"));
  }
  //		con.executeNonQuery(string("use hive"));
  sql::Connection con(config::Config::getProperty("db.connection"));
  util::StringTokenizer st(sql, ";");
  std::string del("\n");
  int tok = st.countTokens();
  for (int i = 0; i < tok - 1; i++) {
    std::string t = st.nextToken();
    t = StringUtil::trim(t, del);
    std::string next = StringUtil::trim(t);
    LOGDEBUG("Create Table " << next);
    LOGDEBUG("Create Table leng" << next.size());
    if (next.size() > 1)
      con.executeNonQuery(next);
  }
}

