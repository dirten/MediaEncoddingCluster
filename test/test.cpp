
#include "org/esb/hive/Setup.h"
#include "org/esb/config/config.h"
int main(){
  org::esb::config::Config::setProperty("db.connection","mysql://host=;db=hive2;user=root");
  org::esb::hive::Setup::buildDatabaseModel("../sql/hive-0.0.2.sql");
  return 0;
}

