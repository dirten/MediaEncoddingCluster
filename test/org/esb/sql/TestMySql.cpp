/* 
 * File:   TestMySql.cpp
 * Author: jhoelscher
 *
 * Created on 7. Dezember 2009, 13:15
 */

#include <stdlib.h>

#include "org/esb/hive/DatabaseService.h"
#include "org/esb/config/config.h"
#include "org/esb/util/Log.h"
#include "org/esb/sql/my_sql.h"
/*
 * 
 */
using namespace org::esb::hive;
using namespace org::esb::config;
using namespace org::esb::util;
int main(int argc, char** argv) {
  Log::open("");
  std::string src = MEC_SOURCE_DIR;
  std::string datadir="--datadir=";
  datadir+=src;

  static char *server_args[] = {
    "this_program",       /* this string is not used */
    const_cast<char*>(datadir.c_str()),
    "--key_buffer_size=32M"
  };
  static char *server_groups[] = {
    "embedded",
    "server",
    "this_program_SERVER",
    (char *)NULL
  };

    if (mysql_library_init(sizeof(server_args) / sizeof(char *),
                          server_args, server_groups)) {
      fprintf(stderr, "could not initialize MySQL library\n");
    }
    mysql_debug("d:t:O,/tmp/client.trace");




//  DatabaseService::start(src);


  Config::setProperty("hive.base_path", src.c_str());



//  mysql_thread_init();
  MYSQL * handle=mysql_init(NULL);
  if(!mysql_real_connect(handle, "localhost", "root", "", "", 0, NULL, 0)){
    LOGERROR("could not connect to database"<<std::string(mysql_error(handle)));
  }
  MYSQL_STMT * stmt = mysql_stmt_init(handle);
  std::string select="select now(), now(), now()";
  LOGDEBUG("Statement->state="<<stmt->state);
  LOGDEBUG("Statement->memroot->free->size="<<stmt->mem_root.free);
  LOGDEBUG("MYSQL_STMT_INIT_DONE="<<MYSQL_STMT_INIT_DONE);
  free(stmt->mem_root.free);
  if(mysql_stmt_prepare(stmt, select.c_str(), select.length())){
    LOGERROR("could not connect to database"<<std::string(mysql_stmt_error(stmt)));
  }
  LOGDEBUG("Statement->memroot->free->size="<<stmt->mem_root.free);


  mysql_stmt_close(stmt);
  mysql_close(handle);
//  mysql_thread_end();
  mysql_library_end();

//  DatabaseService::stop();
  Log::close();
  Config::close();
  return (EXIT_SUCCESS);
}

