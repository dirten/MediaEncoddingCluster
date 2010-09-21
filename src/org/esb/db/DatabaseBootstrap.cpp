/* 
 * File:   DatabaseBootstrap.cpp
 * Author: HoelscJ
 * 
 * Created on 20. September 2010, 16:58
 */

#include "DatabaseBootstrap.h"
#include "org/esb/lang/Process.h"
#include "org/esb/config/config.h"
using namespace org::esb::lang;
namespace org {
  namespace esb {
    namespace db {

      void DatabaseBootstrap::bootstrap() {
        std::string mysql_bin=org::esb::config::Config::get("MYSQLD_BIN");
        std::string mysql_data=org::esb::config::Config::get("MYSQL_DATA");
        std::string mysql_lang=org::esb::config::Config::get("MYSQL_LANG");
        std::list<std::string> args;
        args.push_back(std::string("-c"));
        std::string cmd_line=mysql_bin;
        cmd_line+=" --verbose";
        cmd_line+=" --bootstrap";
        cmd_line+=" --datadir="+mysql_data;
        cmd_line+=" --language="+mysql_lang;
        cmd_line+=" < /home/HoelscJ/devel/mec/sql/CreateDatabase.sql.sql";

//        args.push_back(std::string("../target/dependency/bin/mysqld --verbose --bootstrap --datadir=. < /home/HoelscJ/devel/mec/sql/CreateDatabase.sql.sql"));
        args.push_back(cmd_line);
//        args.push_back(std::string("--verbose"));
//        args.push_back(std::string("--bootstrap"));
//        args.push_back(std::string("--datadir=."));
//        args.push_back(std::string("</home/HoelscJ/devel/mec/sql/CreateDatabase.sql"));
        Process p1("/bin/sh", args);
        p1.start();
      }

      DatabaseBootstrap::DatabaseBootstrap() {
      }

      DatabaseBootstrap::DatabaseBootstrap(const DatabaseBootstrap& orig) {
      }

      DatabaseBootstrap::~DatabaseBootstrap() {
      }
    }
  }
}
