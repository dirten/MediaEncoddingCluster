/* 
 * File:   TestSqlConnection.cpp
 * Author: jhoelscher
 *
 * Created on 13. Oktober 2009, 10:52
 */

#include <stdlib.h>
#include <string>
#include "org/esb/config/config.h"
#include "org/esb/util/Log.h"
#include "org/esb/sql/Connection.h"
#include "org/esb/sql/Statement.h"
#include "org/esb/sql/PreparedStatement.h"
#include "org/esb/sql/ResultSet.h"

#include "org/esb/hive/DatabaseService.h"
#include <boost/thread.hpp>
#include <boost/bind.hpp>

#include "org/esb/lang/Thread.h"

/*
 * 
 */
using namespace org::esb;

void mysql_thread() {
  for (int a = 0; a < 1000; a++) {
    sql::Connection con("");
    con.executeNonQuery("use testdb");
    con.executeNonQuery("insert into test (a,b) values (1,2)");
  }
}

void test_single_statement() {
  sql::Connection con("");
  con.executeNonQuery("use testdb");
  sql::Statement stmt=con.createStatement("select * from test");
  sql::ResultSet rs=stmt.executeQuery();

  while(rs.next()){
//    std::cout << "Rs"<<rs.getString(1)<<std::endl;
  }
/*
  sql::Statement * stmt2=new sql::Statement(con.createStatement("select * from test"));
  sql::ResultSet rs2=stmt2->executeQuery();
  while(rs2.next()){
//    std::cout << "Rs"<<rs.getString(1)<<std::endl;
  }


  delete stmt2;*/
}
void test_execute_non_query() {
  sql::Connection con("");
  //  con.executeNonQuery("create database testdb");
  con.executeNonQuery("use testdb");
  //  con.executeNonQuery("create table test (a integer(11), b integer(11))");
  con.executeNonQuery("insert into test (a,b) values (1,2)");
  //  sql::Statement stmt=con.createStatement("select * from test");
  //  sql::ResultSet rs=stmt.executeQuery();
  //  stmt.close();
  //  con.executeNonQuery("drop database testdb");
  //  delete rs;
  //  delete stmt;
}

void test_multi_connections() {
  sql::Connection con("");
  //  con.executeNonQuery("create database testdb");
  //  con.executeNonQuery("use testdb");
  //  con.executeNonQuery("create table test (a integer(11), b integer(11))");
  for (int a = 0; a < 250; a++) {
    boost::thread t(boost::bind(&mysql_thread));
  }
  Thread::sleep2(50000);

  //  con.executeNonQuery("drop database testdb");
}
sql::Connection * test_con=NULL;
sql::PreparedStatement * pstmt=NULL;

void test_insert_thread(){
  mysql_thread_init();
  pstmt->setInt("a",1);
  pstmt->setInt("b",2);
  pstmt->execute();
  mysql_thread_end();
}

int main(int argc, char** argv) {
  std::string src = MEC_SOURCE_DIR;
  config::Config::setProperty("hive.base_path", src.c_str());
  hive::DatabaseService::start(MEC_SOURCE_DIR);
  mysql_thread_init();
  {
    sql::Connection con("mysql:host=;db=;user=;passwd=");

    con.executeNonQuery("create database testdb;");

    con.executeNonQuery("use testdb");
    con.executeNonQuery("create table test (a integer(11), b integer(11))");
    test_con=new sql::Connection("mysql:db=testdb");
    pstmt=test_con->prepareStatement2("insert into test (a,b) values (:a,:b)");
    boost::thread t(boost::bind(&test_insert_thread));
    t.join();
    for (int a = 0; a < 1; a++){
      test_execute_non_query();
      test_single_statement();
    }

    delete pstmt;
    delete test_con;
    //  test_multi_connections();

    con.executeNonQuery("drop database testdb");

    con.close();
  }
  org::esb::config::Config::close();
  Log::close();

  mysql_thread_end();
  hive::DatabaseService::stop();
  return (EXIT_SUCCESS);
}

