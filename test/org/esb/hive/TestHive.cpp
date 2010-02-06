/* 
 * File:   TestHive.cpp
 * Author: jhoelscher
 *
 * Created on 2. Februar 2010, 11:07
 */

#include <stdlib.h>

#include "org/esb/hive/HiveListener.h"
#include "org/esb/hive/HiveClient.h"
#include "org/esb/hive/HiveClientAudio.h"

#include "org/esb/io/File.h"
#include "org/esb/io/FileInputStream.h"
#include "org/esb/util/StringTokenizer.h"
#include "org/esb/util/StringUtil.h"
#include "org/esb/hive/DatabaseService.h"
#include "org/esb/util/StringUtil.h"
#include "org/esb/hive/FileImporter.h"
#include "org/esb/hive/FileExporter.h"
#include "org/esb/hive/JobUtil.h"
#include "org/esb/hive/job/ProcessUnitWatcher.h"
#include "org/esb/signal/Messenger.h"
#include "org/esb/signal/Message.h"
#include "org/esb/lang/Thread.h"
#include "org/esb/config/config.h"

#include "org/esb/sql/Connection.h"
#include "org/esb/sql/Statement.h"
#include "org/esb/sql/ResultSet.h"
#include "org/esb/util/Log.h"


/*
 * 
 */


void createDatabase() {
  using namespace org::esb;
  std::string src = MEC_SOURCE_DIR;
  std::string sqlfile = src;
  sqlfile.append("/sql/hive-0.0.3.sql");
  logdebug("loading DB model from " << sqlfile);
  std::string sql;
  io::File f(sqlfile.c_str());
  io::FileInputStream(&f).read(sql);
  //  logdebug("FullSql:" << sql);
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
    logdebug("Create Table " << next);
    logdebug("Create Table leng" << next.size());
    if (next.size() > 1)
      con.executeNonQuery(next);
  }
}

void deleteDatabase() {
  using namespace org::esb;
  std::string src = MEC_SOURCE_DIR;
  std::string sqlfile = src;

  {
    sql::Connection con_a("");
    con_a.executeNonQuery(string("DROP DATABASE hive"));
  }

}

void loadTestFile(std::string testfile, std::string p) {
  using namespace org::esb;
  char * argv[] = {"", (char*) testfile.c_str()};
  //  logdebug("TestFileIMport");
  int fileid = import(2, argv);
  std::string file = util::StringUtil::toString(fileid);
  std::string profile = p;
  std::string outdir = MEC_SOURCE_DIR;
  outdir += "/export/";
  char * jobarg[] = {"", "", (char*) file.c_str(), (char*) profile.c_str(), (char*) outdir.c_str()};
  jobcreator(4, jobarg);
  //  sql::Connection con_a("mysql:host=;db=hive;user=;passwd=");
  //  con_a.executeNonQuery(string("delete from streams where fileid=2 and stream_type=1"));
  //  con_a.executeNonQuery(string("update streams set stream_index=0 where fileid=2 and stream_type=1"));
}

int main(int argc, char** argv) {
  using namespace org::esb;
  using namespace org::esb::signal;
  /**
   * opening log for stdout
   */
  std::string logconfigpath=MEC_SOURCE_DIR;
  logconfigpath.append("/res");
  Log::open(logconfigpath);
//  Log::open("");
  /**
   * setting base directory, nedded by some libs
   */
  std::string src = MEC_SOURCE_DIR;
  config::Config::setProperty("hive.base_path", src.c_str());

  /**
   * create tmp dir to receive the encoded Packets
   */
  std::string tmp_path = MEC_SOURCE_DIR;
  tmp_path.append("/tmp");
  org::esb::io::File tpath(tmp_path);
  if (!tpath.exists())
    tpath.mkdir();
  /**
   * starting database service
   */
  config::Config::setProperty("db.connection", "mysql:host=;db=hive;user=;passwd=");
  hive::DatabaseService::start(MEC_SOURCE_DIR);
  hive::DatabaseService::thread_init();

  try {
    sql::Connection con("");
    con.executeNonQuery(string("use hive"));
  } catch (sql::SqlException & ex) {
    LOGERROR("org.esb.hive.TestHive", ex.what());
    createDatabase();
    std::string testfile = MEC_SOURCE_DIR;
    testfile += "/test.dvd";
    if (argc > 1)
      testfile = argv[1];
    /**
     * loading testfile into hive
     */
    loadTestFile(testfile, argc>2?argv[2]:"1");
  }

  /**
   * starting hive for the client
   */
  config::Config::setProperty("hive.port", "20200");

  org::esb::hive::HiveListener hive;
  Messenger::getInstance().addMessageListener(hive);
  org::esb::hive::job::ProcessUnitWatcher puw;
  Messenger::getInstance().addMessageListener(puw);

  Messenger::getInstance().sendMessage(Message().setProperty("processunitwatcher", org::esb::hive::START));
  Messenger::getInstance().sendMessage(Message().setProperty("hivelistener", org::esb::hive::START));


  /**
   * starting the hiveclient
   */
  string host = org::esb::config::Config::getProperty("client.host", "localhost");
  int port = atoi(org::esb::config::Config::getProperty("client.port", "20200"));

  org::esb::hive::HiveClient client(host, port);
  Messenger::getInstance().addMessageListener(client);
  org::esb::hive::HiveClientAudio clientaudio(host, port);
  Messenger::getInstance().addMessageListener(clientaudio);

  Messenger::getInstance().sendRequest(Message().setProperty("hiveclient", org::esb::hive::START));
  Messenger::getInstance().sendRequest(Message().setProperty("hiveclientaudio", org::esb::hive::START));


  /**
   * waiting while the encoding is done
   */
  //  ctrlCHitWait();

  bool wait = true;
  while (wait) {
    sql::Connection con("mysql:host=;db=hive;user=;passwd=");
    sql::Statement stmt = con.createStatement("select * from jobs where id=1");
    sql::ResultSet rs = stmt.executeQuery();
    if (rs.next()) {
      if (rs.getString("status") == "completed")
        wait = false;
      lang::Thread::sleep2(2000);
    } else {
      LOGERROR("org.esb.hive.TestHive", "result for encoding Job not found");
      wait = false;
    }
  }
  LOGDEBUG("org.esb.hive.TestHive", "file completed, continue to export the file");

  FileExporter::exportFile(2);
  //  tpath.deleteFile();


  /*
   *
   * Stopping Application Services from configuration
   *
   */
  Messenger::getInstance().sendRequest(Message().setProperty("hiveclient", org::esb::hive::STOP));
  Messenger::getInstance().sendRequest(Message().setProperty("hiveclientaudio", org::esb::hive::STOP));
  Messenger::getInstance().sendRequest(Message().setProperty("processunitwatcher", org::esb::hive::STOP));
  Messenger::getInstance().sendRequest(Message().setProperty("hivelistener", org::esb::hive::STOP));

  Messenger::free();


  deleteDatabase();
  hive::DatabaseService::thread_end();

  hive::DatabaseService::stop();
  Log::close();
  config::Config::close();
  return (EXIT_SUCCESS);
}

