#include "org/esb/sql/Connection.h"
#include "org/esb/config/config.h"
#include "org/esb/hive/DatabaseService.h"

using namespace org::esb;

int main() {
  std::string src = MEC_SOURCE_DIR;
  std::string sqlfile = src;

  config::Config::setProperty("hive.base_path", src.c_str());
  config::Config::setProperty("db.connection", "mysql:host=;db=hive;user=;passwd=");
  hive::DatabaseService::start(MEC_SOURCE_DIR);

  //config::Config::setProperty("db.connection","")
  {
    sql::Connection con_a("");
    con_a.executeNonQuery(string("DROP DATABASE hive"));
  }

}
