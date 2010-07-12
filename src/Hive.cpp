/*----------------------------------------------------------------------
 *  File    : Hive.cpp
 *  Author  : Jan Hölscher <jan.hoelscher@esblab.com>
 *  Purpose :
 *  Created : 2007, 12:30 by Jan Hölscher <jan.hoelscher@esblab.com>
 *
 *
 * MediaEncodingCluster, Copyright (C) 2001-2009   Jan Hölscher
 *
 * This program License under the terms in the LICENSE file
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  
 *
 * ----------------------------------------------------------------------
 */

#include "config.h"
#include <boost/program_options.hpp>
#include <boost/asio.hpp>
#include "org/esb/config/config.h"
#include "org/esb/hive/job/ProcessUnitWatcher.h"
#include "org/esb/hive/JobScanner.h"
#include "org/esb/web/WebServer.h"
#include "org/esb/hive/DirectoryScanner.h"
#include "org/esb/av/AV.h"

#include "org/esb/hive/FileImporter.h"
#include "org/esb/hive/FileExporter.h"
#include "org/esb/hive/ExportScanner.h"
#include "org/esb/hive/DatabaseService.h"
#include "org/esb/util/Decimal.h"
#include "org/esb/hive/HiveClient.h"
#include "org/esb/hive/HiveClientAudio.h"
#include "org/esb/hive/HiveListener.h"
#include "org/esb/hive/Setup.h"
#include "org/esb/signal/Messenger.h"
#include "org/esb/signal/Message.h"
#include "org/esb/sql/Connection.h"
#include "org/esb/sql/Statement.h"
#include "org/esb/sql/PreparedStatement.h"
#include "org/esb/sql/ResultSet.h"
#include "org/esb/util/StringUtil.h"
#if !defined(_WIN32)

#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>
#else
#include <windows.h>
#include <tchar.h>
#include <strsafe.h>
#include "client/windows/handler/exception_handler.h"
#endif  // !_WIN32
#include "org/esb/util/Log.h"
#include "org/esb/lang/StackDumper.h"
#include "org/esb/hive/NodeResolver.h"
#define TO_STRING(s) #s
using namespace org::esb;
using namespace org::esb::net;
using namespace org::esb::io;
using namespace org::esb::config;
using namespace org::esb::hive;

using namespace org::esb::web;
using namespace org::esb::util;
using namespace org::esb::signal;
//using namespace std;


using namespace std;

namespace po = boost::program_options;

void listener(int argc, char * argv[]);
void client(int argc, char * argv[]);
void shell(int argc, char * argv[]);
void start();
int rec = 0;

int main(int argc, char * argv[]) {

  /*setting default path to Program*/
  org::esb::io::File f(argv[0]);

  std::string s = f.getFilePath();
  char * path = new char[s.length() + 1];
  memset(path, 0, s.length() + 1);
  strcpy(path, s.c_str());

  Config::setProperty("hive.path", path);

  std::string sb = org::esb::io::File(f.getParent()).getParent();
  char * base_path = new char[sb.length() + 1];
  memset(base_path, 0, sb.length() + 1);
  strcpy(base_path, sb.c_str());
  std::string logconfigpath = sb;
  logconfigpath.append("/res");

  Log::open(logconfigpath);

  std::string dump_path = sb;
  dump_path.append("/dmp");
  org::esb::io::File dpath(dump_path);
  if (!dpath.exists())
    dpath.mkdir();

  Config::setProperty("hive.dump_path", dump_path.c_str());
  //    std::wstring wdump_path(dump_path.begin(), dump_path.end());
//  new StackDumper(dump_path);
  std::string tmp_path = sb;
  tmp_path.append("/tmp");
  org::esb::io::File tpath(tmp_path);
  if (!tpath.exists())
    tpath.mkdir();

  Config::setProperty("hive.base_path", base_path);
  try {

    std::string config_path = Config::getProperty("hive.base_path");
    config_path.append("/.hive.cfg");
    po::options_description gen("General options");

    gen.add_options()("help", "produce this message")("config", po::value<std::string > ()->default_value(config_path), "use Configuration File")("version", "Prints the Version");

    po::options_description inst("Install options");
    inst.add_options()
        ("install", "install the server instance on this node")
        ("hiveport", po::value<int>()->default_value(20200), "on which port will the hive be listen on")
        ("webport", po::value<int>()->default_value(8080), "on which port will the web admin be listen on")
        ;
    po::options_description ser("Server options");
    ser.add_options()("daemon,d", "start the Hive as Daemon Process")("run,r", "start the Hive as Console Process")
        ;

    po::options_description cli("Client options");
    cli.add_options()("client,i", "start the Hive Client")("host,h", po::value<std::string > ()->default_value("localhost"), "Host to connect")("port,p", po::value<int>()->default_value(20200), "Port to connect");

    po::options_description all("all");
    all. add(gen). add(ser). add(cli).add(inst);

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, all), vm);
    po::notify(vm);

    if (vm.count("help") || argc == 1) {
      cout << all << "\n";
      return 1;
    }
    av_register_all();
    avcodec_init();
    avcodec_register_all();
    config::Config::setProperty("db.connection", "mysql:host=;db=hive;user=;passwd=");

    if (vm.count("install")) {

      /**starting the internal database service*/
      org::esb::hive::DatabaseService::start(base_path);

      /*creating the database */
      try {
        /*check if database exists*/
        sql::Connection con(config::Config::getProperty("db.connection"));
        con.executeNonQuery(string("use hive"));
        /*if database exists the try to select the model version*/
        sql::Statement stmt = con.createStatement("SELECT * FROM version WHERE component='database.model'");
        sql::ResultSet rs = stmt.executeQuery();
        if (rs.next()) {
          std::string ver = rs.getString("version");
          if (ver == "0.0.4") {
            LOGINFO("Database version is 0.0.4, update needed!!!");
            std::string sql_script = std::string(org::esb::config::Config::getProperty("hive.path"));
            sql_script.append("/../sql/hive-0.0.5.sql");
            hive::Setup::buildDatabaseModel(sql_script.c_str());
          }else if(ver == "0.0.5") {
            LOGINFO("Database is up to date");
          }
        } else {
          /**Update database model to ver 0.0.4*/
          std::string sql_script004 = std::string(org::esb::config::Config::getProperty("hive.path"));
          sql_script004.append("/../sql/hive-0.0.4.sql");
          hive::Setup::buildDatabaseModel(sql_script004.c_str());
          /**Update database model to ver 0.0.5*/
          std::string sql_script005 = std::string(org::esb::config::Config::getProperty("hive.path"));
          sql_script005.append("/../sql/hive-0.0.5.sql");
          hive::Setup::buildDatabaseModel(sql_script005.c_str());
        }
      } catch (sql::SqlException & ex) {
        /*when not exists then create it*/
        LOGERROR("seems to be the first installation, creating the database now");
        try {
          sql::Connection con_create(std::string(""));
          con_create.executeNonQuery(string("CREATE DATABASE hive"));
          con_create.executeNonQuery(string("use hive"));
          /** retrieving the database schema name from filesystem* */
          std::string sql_script = std::string(org::esb::config::Config::getProperty("hive.path"));
          sql_script.append("/../sql/hive-0.0.3.sql");

          /**build database model*/
          hive::Setup::buildDatabaseModel(sql_script.c_str());

          /**Update database model to ver 0.0.4*/
          sql_script = std::string(org::esb::config::Config::getProperty("hive.path"));
          sql_script.append("/../sql/hive-0.0.4.sql");
          hive::Setup::buildDatabaseModel(sql_script.c_str());

          /**Update database model to ver 0.0.5*/
          std::string sql_script005 = std::string(org::esb::config::Config::getProperty("hive.path"));
          sql_script005.append("/../sql/hive-0.0.5.sql");
          hive::Setup::buildDatabaseModel(sql_script005.c_str());

          std::map<std::string, std::string> conf;
          conf["hive.mode"] = "server";
          conf["hive.port"] = StringUtil::toString(vm["hiveport"].as<int> ());
          conf["web.port"] = StringUtil::toString(vm["webport"].as<int> ());

          conf["hive.start"] = "true";
          conf["web.start"] = "true";
          conf["hive.autoscan"] = "true";
          conf["hive.scaninterval"] = "30";
          std::string webroot = std::string(Config::getProperty("hive.base_path"));
          webroot.append("/web");
          conf["web.docroot"] = webroot;

          sql::PreparedStatement * pstmt = con_create.prepareStatement2("replace into config(config_key, config_val) values (:key, :val)");
          std::map<std::string, std::string>::iterator it = conf.begin();
          for (; it != conf.end(); it++) {
            pstmt->setString("key", it->first);
            pstmt->setString("val", it->second);
            pstmt->execute();
            LOGDEBUG("key=" << it->first << " val=" << it->second);
          }
          delete pstmt;
        } catch (sql::SqlException & ex) {
          LOGERROR("SqlException:" << ex.what());
          return -1;
        }
        /**stopping the internal database service*/
        org::esb::hive::DatabaseService::stop();
      }
    }

    if (vm.count("run")) {
      LOGDEBUG("start mhive server");
      org::esb::hive::DatabaseService::start(base_path);
      if (!Config::init((char*) vm["config"].as<std::string > ().c_str())) {
        LOGERROR("could not load config from Database, exiting!!!");
      }
      std::string webroot = std::string(Config::getProperty("hive.base_path"));
      webroot.append("/web");
      Config::setProperty("web.docroot", webroot.c_str());
      listener(argc, argv);
    }

    if (vm.count("client")) {
      Config::setProperty("client.port", Decimal(vm["port"].as<int> ()).toString().c_str());
      Config::setProperty("client.host", vm["host"].as<std::string > ().c_str());
      client(argc, argv);
    }

    if (vm.count("version")) {
      cout << "MediaEncodingCluster " << MHIVE_VERSION << endl;
      //    cout << org::esb::hive::VERSION_STRING<< endl;
      cout << LIBAVCODEC_IDENT << endl;
      cout << LIBAVFORMAT_IDENT << endl;
      cout << LIBAVUTIL_IDENT << endl;
      cout << LIBSWSCALE_IDENT << endl;
      exit(0);
    }
  } catch (Exception * e) {
    std::cerr << "error: " << e->what() << "\n";
    return 1;

  }
  delete[] path;
  delete[] base_path;
  org::esb::hive::DatabaseService::stop();

  org::esb::config::Config::close();
  LOGINFO("MHive is not running anymore!!!")
  Log::close();

  return 0;
}

#ifdef WIN32


  boost::mutex terminationMutex;
  boost::condition ctrlCHit;
  boost::condition serverStopped;

  BOOL WINAPI console_ctrl_handler(DWORD ctrl_type) {
    switch (ctrl_type) {
      case CTRL_C_EVENT:
      case CTRL_BREAK_EVENT:
      case CTRL_CLOSE_EVENT:
      case CTRL_SHUTDOWN_EVENT:
      {
        boost::mutex::scoped_lock terminationLock(terminationMutex);
        LOGDEBUG("ctlc event");
        ctrlCHit.notify_all(); // should be just 1

        //      serverStopped.wait(terminationLock);
        return TRUE;
      }
      default:
        return FALSE;
    }
  }

  void ctrlCHitWait() {
    SetConsoleCtrlHandler(console_ctrl_handler, TRUE);
    boost::mutex::scoped_lock terminationLock(terminationMutex);
    ctrlCHit.wait(terminationLock);
  }
#else

void ctrlCHitWait() {
  sigset_t wait_mask2;
  sigemptyset(&wait_mask2);
  sigaddset(&wait_mask2, SIGINT);
  sigaddset(&wait_mask2, SIGQUIT);
  sigaddset(&wait_mask2, SIGTERM);
  sigaddset(&wait_mask2, SIGCHLD);
  pthread_sigmask(SIG_BLOCK, &wait_mask2, 0);
  int sig = 0;
  //sigdelset(&wait_mask, SIGCHLD);

  int err;
  do {
    err = sigwait(&wait_mask2, &sig);
  } while (err != 0);

}
#endif

class NodeAgent : public NodeListener {

    classlogger("NodeListener")
  public:
    NodeAgent() {
      having_server = false;
    }

    void onNodeUp(Node & node) {
      LOGDEBUG("NodeUp:" << node.toString());
      LOGDEBUG("NodeData " << node.getData("type"));
      if (node.getData("type") == "server" && !having_server) {
        string host = node.getIpAddress().to_string();
        int port = atoi(node.getData("port").c_str());
        LOGINFO("Server Node:" << node.toString());
        /**
         * @TODO: this is a memleak here, the created objects must be deleted
         *
         */
        org::esb::hive::HiveClient *client = new org::esb::hive::HiveClient(host, port);
        Messenger::getInstance().addMessageListener(*client);

        org::esb::hive::HiveClientAudio *clientaudio = new org::esb::hive::HiveClientAudio(host, port);
        Messenger::getInstance().addMessageListener(*clientaudio);

        Messenger::getInstance().sendRequest(Message().setProperty("hiveclient", org::esb::hive::START));
        Messenger::getInstance().sendRequest(Message().setProperty("hiveclientaudio", org::esb::hive::START));
        having_server = true;
        /*
         org::esb::hive::HiveClient client(host, port);
         client.start();
         */
      }
    }

    void onNodeDown(Node & node) {
      LOGWARN("NodeDown:" << node.toString());
      if (node.getData("type") == "server") {
        //      LOGWARN("ServerNode is Down freeing client:" << node.toString());
        /*
         Messenger::getInstance().sendRequest(Message().setProperty("hiveclient",
         org::esb::hive::STOP));
         Messenger::getInstance().sendRequest(Message().setProperty(
         "hiveclientaudio", org::esb::hive::STOP));
         Messenger::free();*/
      }
    }
  private:
    bool having_server;
  };

  void client(int argc, char *argv[]) {
    org::esb::hive::Node node;
    node.setData("type", "client");
    node.setData("version", MHIVE_VERSION);
    org::esb::hive::NodeResolver res(boost::asio::ip::address::from_string("0.0.0.0"), boost::asio::ip::address::from_string("239.255.0.1"), 6000, node);
    NodeAgent agent;
    res.setNodeListener(&agent);
    res.start();
    ctrlCHitWait();

    Messenger::getInstance().sendRequest(Message().setProperty("hiveclient", org::esb::hive::STOP));
    Messenger::getInstance().sendRequest(Message().setProperty("hiveclientaudio", org::esb::hive::STOP));
    Messenger::free();
  }

  /*----------------------------------------------------------------------------------------------*/
  void start() {

    /*
     *
     * Initializing Application Services
     *
     */

    //  org::esb::hive::DatabaseService dbservice(org::esb::config::Config::getProperty("hive.base_path"));
    //  Messenger::getInstance().addMessageListener(dbservice);

    org::esb::hive::JobScanner jobscan;
    Messenger::getInstance().addMessageListener(jobscan);

    org::esb::hive::DirectoryScanner dirscan;
    Messenger::getInstance().addMessageListener(dirscan);

    org::esb::hive::ExportScanner expscan;
    Messenger::getInstance().addMessageListener(expscan);

    org::esb::web::WebServer webserver;
    Messenger::getInstance().addMessageListener(webserver);

    org::esb::hive::HiveListener hive;
    Messenger::getInstance().addMessageListener(hive);

    org::esb::hive::job::ProcessUnitWatcher puw;
    Messenger::getInstance().addMessageListener(puw);

    string host = org::esb::config::Config::getProperty("client.host", "localhost");
    int port = atoi(org::esb::config::Config::getProperty("client.port", "20200"));
    org::esb::hive::HiveClient client(host, port);
    Messenger::getInstance().addMessageListener(client);

    org::esb::hive::HiveClientAudio clientaudio(host, port);
    Messenger::getInstance().addMessageListener(clientaudio);

    /*
     *
     * Starting Application Services from configuration
     *
     */

    //  Messenger::getInstance().sendMessage(Message().setProperty("databaseservice", org::esb::hive::START));

    if (string(org::esb::config::Config::getProperty("hive.start")) == "true") {
      string base_path = org::esb::config::Config::getProperty("hive.base_path");
      Messenger::getInstance().sendMessage(Message().setProperty("processunitwatcher", org::esb::hive::START));
      //    Messenger::getInstance().sendMessage(Message().setProperty("jobwatcher", org::esb::hive::START));
      Messenger::getInstance().sendMessage(Message().setProperty("hivelistener", org::esb::hive::START));
    }

    if (string(org::esb::config::Config::getProperty("web.start")) == "true" || string(org::esb::config::Config::getProperty("hive.mode")) == "setup") {
      Messenger::getInstance().sendRequest(Message().setProperty("webserver", org::esb::hive::START));
    }

    if (string(org::esb::config::Config::getProperty("hive.autoscan")) == "true") {
      Messenger::getInstance().sendMessage(Message(). setProperty("directoryscan", org::esb::hive::START). setProperty("directory", org::esb::config::Config::getProperty("hive.scandir")). setProperty("interval", org::esb::config::Config::getProperty("hive.scaninterval")));
      Messenger::getInstance().sendRequest(Message().setProperty("exportscanner", org::esb::hive::START));
      Messenger::getInstance().sendMessage(Message().setProperty("jobscanner", org::esb::hive::START));

    }
    if (string(org::esb::config::Config::getProperty("mode.client")) == "On") {
      Messenger::getInstance().sendRequest(Message().setProperty("hiveclient", org::esb::hive::START));
      Messenger::getInstance().sendRequest(Message().setProperty("hiveclientaudio", org::esb::hive::START));
    }

    ctrlCHitWait();
    LOGINFO("shutdown app, this will take some time!");
    /*
     *
     * Stopping Application Services from configuration
     *
     */
    Messenger::getInstance().sendRequest(Message().setProperty("hiveclient", org::esb::hive::STOP));
    Messenger::getInstance().sendRequest(Message().setProperty("hiveclientaudio", org::esb::hive::STOP));
    Messenger::getInstance().sendRequest(Message().setProperty("jobscanner", org::esb::hive::STOP));
    Messenger::getInstance().sendRequest(Message().setProperty("directoryscan", org::esb::hive::STOP));
    Messenger::getInstance().sendRequest(Message().setProperty("exportscanner", org::esb::hive::STOP));
    Messenger::getInstance().sendRequest(Message().setProperty("jobwatcher", org::esb::hive::STOP));
    Messenger::getInstance().sendRequest(Message().setProperty("processunitwatcher", org::esb::hive::STOP));
    Messenger::getInstance().sendRequest(Message().setProperty("webserver", org::esb::hive::STOP));
    Messenger::getInstance().sendRequest(Message().setProperty("databaseservice", org::esb::hive::STOP));
    Messenger::getInstance().sendRequest(Message().setProperty("hivelistener", org::esb::hive::STOP));

    Messenger::free();
    //  mysql_library_end();
  }

  void listener(int argc, char *argv[]) {
    org::esb::hive::Node node;
    node.setData("type", "server");
    node.setData("version", MHIVE_VERSION);
    node.setData("port", org::esb::config::Config::getProperty("client.port", "20200"));
    org::esb::hive::NodeResolver res(boost::asio::ip::address::from_string("0.0.0.0"), boost::asio::ip::address::from_string("239.255.0.1"), 6000, node);
    res.start();

    /**starting the server main services*/
    start();

  }

