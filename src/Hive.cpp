//#include <iostream>
//#include <stdlib.h>
#include <boost/program_options.hpp>
#include "org/esb/signal/Messenger.h"
//#include "org/esb/lang/Exception.h"
#include "org/esb/signal/Message.h"
//#include "org/esb/signal/Messenger.h"

//#include "org/esb/net/TcpServerSocket.h"
//#include "org/esb/net/TcpSocket.h"
//#include "org/esb/lang/Thread.h"
//#include "org/esb/hive/ProtocolServer.h"
#include "org/esb/config/config.h"
//#include "org/esb/hive/job/JobWatcher.h"
#include "org/esb/hive/job/ProcessUnitWatcher.h"
//#include "org/esb/hive/job/ProcessUnit.h"
//#include "org/esb/io/ObjectInputStream.h"
//#include "org/esb/io/ObjectOutputStream.h"
#include "org/esb/web/WebServer.h"
#include "org/esb/hive/HiveListener.h"
#include "org/esb/hive/DirectoryScanner.h"
#include "org/esb/av/AV.h"
//#include "org/esb/hive/PacketCollector.h"
//#include "Environment.cpp"
#include "org/esb/hive/HiveClient.h"
#include "org/esb/hive/Setup.h"
#include "org/esb/hive/Version.h"

#include "org/esb/hive/FileImporter.h"
#include "org/esb/hive/FileExporter.h"
#include "org/esb/hive/ExportScanner.h"

#include "org/esb/util/Decimal.h"
//#include "org/esb/hive/FileImporter.h"
#include "export.cpp"
//#include "org/esb/util/Log.h"
//#include "job.cpp"
//#include "org/esb/hive/JobUtil.h"
#if !defined(_WIN32)

#  include <signal.h>
#  include <sys/types.h>
#  include <sys/wait.h>
#  include <pthread.h>

#endif  // !_WIN32

using namespace org::esb::net;
using namespace org::esb::io;
using namespace org::esb::config;
using namespace org::esb::hive;
using namespace org::esb::hive::job;
using namespace org::esb::web;
using namespace org::esb::util;
using namespace org::esb::signal;
//using namespace std;



using namespace std;

namespace po = boost::program_options;

void listener(int argc, char * argv[]);
void client(int argc, char * argv[]);
void shell(int argc, char * argv[]);
int rec = 0;

/*
int euclid(int a, int b){
        std::cout<<"cycle("<<a<<","<<b<<std::endl;
        if(b==0)
                return a;
        else
                return euclid(b, a%b);
}
 */
int main(int argc, char * argv[]) {
  //	return 0;
  //    std::cerr << "Path"<<std::endl;
  try
  {
    //    loginit("log.properties");
    //	std::cout<<"eclid(90000,3600)="<<euclid(194400000,375)<<" in "<<rec<<" cycles"<<std::endl;
    //	return 0;

    org::esb::io::File f(argv[0]);
    std::string s = f.getFilePath();
    char * path = new char[s.length() + 1];
    memset(path, 0, s.length() + 1);
    strcpy(path, s.c_str());
    Config::setProperty("hive.path", path);
    std::cout << "Path" << path << std::endl;

    std::string config_path = ".hive.cfg";
    po::options_description gen;

    gen.add_options()
        ("help", "produce this message")
        ("config", po::value<std::string > ()->default_value(config_path), "use Configuration File")
        ("version", "Prints the Version")
        ;


    po::options_description ser("Server options");
    std::string webroot = std::string(Config::getProperty("hive.path"));
    webroot.append("/../web");
    ser.add_options()
        ("server,s", "start the Hive Server Process")
        ("port,p", po::value<int>()->default_value(20200), "specify the port for the Hive Server")
        ("web,w", po::value<int>()->default_value(8080), "start the Web Server Process on the specified port")
        ("webroot,r", po::value<std::string > ()->default_value(webroot), "define the Path for Web Server root")
        ("scandir", po::value<std::string > (), "define the Path to Scan for new Media Files")
        ("scanint", po::value<int>(), "define the Interval to Scan for new Media Files")
        ("database", po::value<std::string > (), "Database Connection mysql://mysql:db=<dbname>;host=<host>;user=<user>;passwd=<user> for e.g. mysql:db=hive;host=localhost;user=root;passwd=test")
        ;


    po::options_description cli("Client options");
    cli.add_options()
        ("client,i", "start the Hive Client")
        ("host,h", po::value<std::string > ()->default_value("localhost"), "Host to connect")
        ("port,p", po::value<int>()->default_value(20200), "Port to connect")
        ;

    po::options_description exp("Export options");
    exp.add_options()
        ("export,e", "Exports a File")
        ("file,f", po::value<std::string > (), "which file to export")
        ("directory,d", po::value<std::string > ()->default_value("."), "Directory in which the File to export")
        ;

    po::options_description imp("Import options");
    imp.add_options()
        ("import,p", "Import a File")
        ("file,f", po::value<std::string > (), "which file to import")
        //      ("directory,d", po::value<std::string > ()->default_value("."), "Directory in which the File to export")
        ;

    po::options_description all("all");
    all.add(gen).add(ser).add(cli).add(exp).add(imp);

    gen.add(ser).add(cli).add(exp);

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, all), vm);
    po::notify(vm);

    if (argc == 1 || vm.count("help")) {
      cout << all << "\n";
      return 1;
    }
    if (vm.count("version")) {
      cout << "MediaEncodingCluster V.-0.0.1" << endl;
      //    cout << org::esb::hive::VERSION_STRING<< endl;
      cout << LIBAVCODEC_IDENT << endl;
      cout << LIBAVFORMAT_IDENT << endl;
      cout << LIBAVUTIL_IDENT << endl;
      cout << LIBSWSCALE_IDENT << endl;
      exit(0);
    }
    av_register_all();
    avcodec_init();
    avcodec_register_all();

    if (vm.count("server")) {
      Config::setProperty("hive.mode", "server");
      if (vm.count("database"))
        Config::setProperty("db.connection", vm["database"].as<std::string > ().c_str());
      try
      {
        Config::init((char*) vm["config"].as<std::string > ().c_str());
      }

      catch(Exception & ex) {
        cout << "Could not open Configuration " << vm["config"].as<std::string > () << endl;
        Config::setProperty("hive.mode", "setup");
      }
      Config::setProperty("config.file", vm["config"].as<std::string > ().c_str());
      Config::setProperty("web.docroot", vm["webroot"].as<std::string > ().c_str());
      if (vm.count("web"))
        Config::setProperty("web.port", Decimal(vm["web"].as<int> ()).toString().c_str());
      if (vm.count("scandir"))
        Config::setProperty("hive.scandir", vm["scandir"].as<std::string > ().c_str());
      if (vm.count("scanint"))
        Config::setProperty("hive.scaninterval", Decimal(vm["scanint"].as<int> ()).toString().c_str());
      Config::setProperty("hive.port", Decimal(vm["port"].as<int>()).toString().c_str());

      listener(argc, argv);
      return 0;
    }

    if (vm.count("client")) {
      Config::setProperty("client.port", Decimal(vm["port"].as<int>()).toString().c_str());
      Config::setProperty("client.host", vm["host"].as<std::string > ().c_str());
      client(argc, argv);
    }
    if (vm.count("export")) {
      Config::init((char*) vm["config"].as<std::string > ().c_str());
      std::string file = vm["file"].as<std::string > ();
      std::string dir = vm["directory"].as<std::string > ();
      exporter((char*) file.c_str(), (char*) dir.c_str());
    }
    if (vm.count("import")) {
      Config::init((char*) vm["config"].as<std::string > ().c_str());
      std::string file = vm["file"].as<std::string > ();

      char * argv[] = {"", (char*) file.c_str()};
      printf("Import File %s", file.c_str());

      int fileid = import(2, argv);
      printf("File imported with ID %i", fileid);
      //    std::string dir = vm["directory"].as<std::string > ();
      //    exporter((char*) file.c_str(), (char*) dir.c_str());
    }

  }

  catch(exception & e) {
    std::cerr << "error: " << e.what() << "\n";
    return 1;

  }
  org::esb::config::Config::close();
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
      logdebug("ctlc event");
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

void client(int argc, char *argv[]) {


  string host = org::esb::config::Config::getProperty("client.host");
  int port = atoi(org::esb::config::Config::getProperty("client.port"));

  org::esb::hive::HiveClient client(host, port);
  Messenger::getInstance().addMessageListener(client);
  Messenger::getInstance().sendRequest(Message().setProperty("hiveclient", org::esb::hive::START));

  ctrlCHitWait();

  Messenger::getInstance().sendRequest(Message().setProperty("hiveclient", org::esb::hive::STOP));

}

/*----------------------------------------------------------------------------------------------*/

void listener(int argc, char *argv[]) {

  //  Setup::check();

  /*
   *
   * Initializing Application Services
   *
   */

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



  /*
   *
   * Starting Application Services from configuration
   *
   */


  if (string(org::esb::config::Config::getProperty("hive.start")) == "true") {
    Messenger::getInstance().sendMessage(Message().setProperty("processunitwatcher", org::esb::hive::START));
    Messenger::getInstance().sendMessage(Message().setProperty("jobwatcher", org::esb::hive::START));
    Messenger::getInstance().sendMessage(Message().setProperty("hivelistener", org::esb::hive::START));
  }

  if (string(org::esb::config::Config::getProperty("web.start")) == "true" ||
      string(org::esb::config::Config::getProperty("hive.mode")) == "setup") {
    Messenger::getInstance().sendRequest(Message().setProperty("webserver", org::esb::hive::START));
  }
  Messenger::getInstance().sendRequest(Message().setProperty("webserver", org::esb::hive::START));

  if (string(org::esb::config::Config::getProperty("hive.autoscan")) == "true") {
    Messenger::getInstance().sendMessage(Message().
        setProperty("directoryscan", org::esb::hive::START).
        setProperty("directory", org::esb::config::Config::getProperty("hive.scandir")).
        setProperty("interval", org::esb::config::Config::getProperty("hive.scaninterval")));
    Messenger::getInstance().sendRequest(Message().setProperty("exportscanner", org::esb::hive::START));
  }

  //  Messenger::getInstance().sendRequest(Message().setProperty("exportscanner", START));


  ctrlCHitWait();

  /*
   *
   * Stopping Application Services from configuration
   *
   */

  Messenger::getInstance().sendRequest(Message().setProperty("directoryscan", org::esb::hive::STOP));
  Messenger::getInstance().sendRequest(Message().setProperty("jobwatcher", org::esb::hive::STOP));
  Messenger::getInstance().sendRequest(Message().setProperty("processunitwatcher", org::esb::hive::STOP));
  Messenger::getInstance().sendRequest(Message().setProperty("hivelistener", org::esb::hive::STOP));
  Messenger::getInstance().sendRequest(Message().setProperty("webserver", org::esb::hive::STOP));
  Messenger::free();
  //  mysql_library_end();

}

