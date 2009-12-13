//#include <iostream>
//#include <stdlib.h>
#include "config.h"
#include <boost/program_options.hpp>
#include "org/esb/signal/Messenger.h"
//#include "org/esb/lang/Exception.h"
#include "org/esb/signal/Message.h"
//#include "org/esb/signal/Messenger.h"

//#include "org/esb/net/TcpServerSocket.h"
//#include "org/esb/net/TcpSocket.h"
//#include "org/esb/lang/Thread.h"
//#include "org/esb/hive/ProtocolServer.h"
#include "org/esb/hive/HiveClient.h"
#include "org/esb/hive/HiveClientAudio.h"
#include "org/esb/config/config.h"
//#include "org/esb/hive/job/JobWatcher.h"
#include "org/esb/hive/job/ProcessUnitWatcher.h"
#include "org/esb/hive/JobScanner.h"
//#include "org/esb/io/ObjectInputStream.h"
//#include "org/esb/io/ObjectOutputStream.h"
#include "org/esb/web/WebServer.h"
#include "org/esb/hive/HiveListener.h"
#include "org/esb/hive/DirectoryScanner.h"
#include "org/esb/av/AV.h"
//#include "org/esb/hive/PacketCollector.h"
//#include "Environment.cpp"
//#include "org/esb/hive/Setup.h"
//#include "org/esb/hive/Version.h"

#include "org/esb/hive/FileImporter.h"
#include "org/esb/hive/FileExporter.h"
#include "org/esb/hive/ExportScanner.h"
#include "org/esb/hive/DatabaseService.h"
#include "org/esb/util/Decimal.h"
//#include "org/esb/hive/FileImporter.h"
//#include "export.cpp"
//#include "job.cpp"
//#include "org/esb/hive/JobUtil.h"
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
#define TO_STRING(s) #s
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

  /*setting default path to Program*/
  org::esb::io::File f(argv[0]);
  
  
  //  logdebug(f.getPath());
  std::string s = f.getFilePath();
  char * path = new char[s.length() + 1];
  memset(path, 0, s.length() + 1);
  strcpy(path, s.c_str());

  Config::setProperty("hive.path", path);

  std::string sb = org::esb::io::File(f.getParent()).getParent();
  char * base_path = new char[sb.length() + 1];
  memset(base_path, 0, sb.length() + 1);
  strcpy(base_path, sb.c_str());

  std::string dump_path = sb;
  dump_path.append("/dmp");
  org::esb::io::File dpath(dump_path);
  if (!dpath.exists())
    dpath.mkdir();

  Config::setProperty("hive.dump_path", dump_path.c_str());
  //  std::wstring wdump_path(dump_path.begin(), dump_path.end());
  //  new StackDumper(dump_path);
  std::string tmp_path = sb;
  tmp_path.append("/tmp");
  org::esb::io::File tpath(tmp_path);
  if (!tpath.exists())
    tpath.mkdir();

  //	google_breakpad::ExceptionHandler exhandler(wdump_path,NULL, NULL, NULL, google_breakpad::ExceptionHandler::HANDLER_ALL);




  Config::setProperty("hive.base_path", base_path);
  //  org::esb::hive::DatabaseService::start(base_path);
  try {
    //    loginit("log.properties");
    //	std::cout<<"eclid(90000,3600)="<<euclid(194400000,375)<<" in "<<rec<<" cycles"<<std::endl;
    //	return 0;



    std::string config_path = Config::getProperty("hive.base_path");
    config_path.append("/.hive.cfg");
    po::options_description gen;

    gen.add_options()
        ("help", "produce this message")
        ("config", po::value<std::string > ()->default_value(config_path), "use Configuration File")
        ("version", "Prints the Version")
        ;


    po::options_description ser("Hive options");
    ser.add_options()
        ("daemon,d", "start the Hive as Daemon Process")
        ("run,r", "start the Hive as Console Process")
        //        ("port,p", po::value<int>()->default_value(20200), "specify the port for the Hive Server")
        //        ("web,w", po::value<int>()->default_value(8080), "start the Web Server Process on the specified port")
        //        ("webroot,r", po::value<std::string > ()->default_value(webroot), "define the Path for Web Server root")
        //        ("scandir", po::value<std::string > (), "define the Path to Scan for new Media Files")
        //        ("scanint", po::value<int>(), "define the Interval to Scan for new Media Files")
        //        ("database", po::value<std::string > (), "Database Connection mysql://mysql:db=<dbname>;host=<host>;user=<user>;passwd=<user> for e.g. mysql:db=hive;host=localhost;user=root;passwd=test")
        ;


    po::options_description cli("Client options");
    cli.add_options()
        ("client,i", "start the Hive Client")
        ("host,h", po::value<std::string > ()->default_value("localhost"), "Host to connect")
        ("port,p", po::value<int>()->default_value(20200), "Port to connect")
        ;
    /*
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
     */

    po::options_description all("all");
    all.
        add(gen).
        add(ser).
        add(cli);
    //      add(exp).
    //      add(imp);

    /*
        gen.
            add(ser).
            add(cli).
            add(exp);
     */
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

    if (vm.count("daemon")) {
      Log::open(Config::getProperty("hive.base_path"));
      logdebug("start daemon");
      org::esb::hive::DatabaseService::start(base_path);
      if (!Config::init((char*) vm["config"].as<std::string > ().c_str())) {
        logdebug("Could not open Configuration, it seems it is the first run " << vm["config"].as<std::string > ());
        Config::setProperty("hive.mode", "setup");
        std::string webroot = std::string(Config::getProperty("hive.base_path"));
        webroot.append("/web");
        Config::setProperty("web.docroot", webroot.c_str());
      }
      Config::setProperty("hive.start_as", "daemon");
      listener(argc, argv);
      //      return 0;
    }
    if (vm.count("run")) {
      logdebug("start console");
      org::esb::hive::DatabaseService::start(base_path);
      if (!Config::init((char*) vm["config"].as<std::string > ().c_str())) {
        logdebug("Could not open Configuration, it seems it is the first run " << vm["config"].as<std::string > ());
        Config::setProperty("hive.mode", "setup");
        std::string webroot = std::string(Config::getProperty("hive.base_path"));
        webroot.append("/web");
        Config::setProperty("web.docroot", webroot.c_str());
      }
      Config::setProperty("hive.start_as", "console");
      //      Config::setProperty("hive.mode", "daemon");
      listener(argc, argv);
      //      return 0;
    }

    if (vm.count("version")) {
      cout << "MediaEncodingCluster V.-" << MHIVE_VERSION << endl;
      //    cout << org::esb::hive::VERSION_STRING<< endl;
      cout << LIBAVCODEC_IDENT << endl;
      cout << LIBAVFORMAT_IDENT << endl;
      cout << LIBAVUTIL_IDENT << endl;
      cout << LIBSWSCALE_IDENT << endl;
      exit(0);
    }
    if (vm.count("client")) {
      Config::setProperty("client.port", Decimal(vm["port"].as<int>()).toString().c_str());
      Config::setProperty("client.host", vm["host"].as<std::string > ().c_str());
      client(argc, argv);
    }
    /*
        if (vm.count("server")) {
          Config::setProperty("hive.mode", "server");
          if (vm.count("database"))
            Config::setProperty("db.connection", vm["database"].as<std::string > ().c_str());
          try {
            Config::init((char*) vm["config"].as<std::string > ().c_str());
          } catch (Exception & ex) {
            cout << "Could not open Configuration " << vm["config"].as<std::string > () << endl;
            Config::setProperty("hive.mode", "setup");
          }
          Config::setProperty("config.file", vm["config"].as<std::string > ().c_str());
          std::string webroot = std::string(Config::getProperty("hive.base_path"));
          webroot.append("/web");
          Config::setProperty("web.docroot", webroot.c_str());
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
     */
  } catch (Exception * e) {
    std::cerr << "error: " << e->what() << "\n";
    return 1;

  }
  delete []path;
  delete []base_path;
  org::esb::hive::DatabaseService::stop();

  org::esb::config::Config::close();
  Log::close();
  //  mysql_server_end();

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
#define SVCNAME TEXT("MHiveService")
SERVICE_STATUS gSvcStatus;
SERVICE_STATUS_HANDLE gSvcStatusHandle;
HANDLE ghSvcStopEvent = NULL;

//VOID SvcInstall(void);
VOID WINAPI SvcCtrlHandler(DWORD);
VOID WINAPI SvcMain(DWORD, LPTSTR *);

VOID ReportSvcStatus(DWORD, DWORD, DWORD);
VOID SvcInit(DWORD, LPTSTR *);
VOID SvcReportEvent(LPTSTR);

VOID SvcReportEvent(LPTSTR szFunction) {
  HANDLE hEventSource;
  LPCTSTR lpszStrings[2];
  TCHAR Buffer[80];

  hEventSource = RegisterEventSource(NULL, SVCNAME);

  if (NULL != hEventSource) {
    StringCchPrintf(Buffer, 80, TEXT("%s failed with %d"), szFunction, GetLastError());

    lpszStrings[0] = SVCNAME;
    lpszStrings[1] = Buffer;

    ReportEvent(hEventSource, // event log handle
        EVENTLOG_ERROR_TYPE, // event type
        0, // event category
        NULL, // event identifier
        NULL, // no security identifier
        2, // size of lpszStrings array
        0, // no binary data
        lpszStrings, // array of strings
        NULL); // no binary data

    DeregisterEventSource(hEventSource);
  }
}

VOID WINAPI SvcCtrlHandler(DWORD dwCtrl) {
  // Handle the requested control code.

  switch (dwCtrl) {
    case SERVICE_CONTROL_STOP:
      ReportSvcStatus(SERVICE_STOP_PENDING, NO_ERROR, 0);

      // Signal the service to stop.

      SetEvent(ghSvcStopEvent);

      return;

    case SERVICE_CONTROL_INTERROGATE:
      // Fall through to send current status.
      break;

    default:
      break;
  }

  ReportSvcStatus(gSvcStatus.dwCurrentState, NO_ERROR, 0);
}

VOID ReportSvcStatus(DWORD dwCurrentState,
    DWORD dwWin32ExitCode,
    DWORD dwWaitHint) {
  static DWORD dwCheckPoint = 1;

  // Fill in the SERVICE_STATUS structure.

  gSvcStatus.dwCurrentState = dwCurrentState;
  gSvcStatus.dwWin32ExitCode = dwWin32ExitCode;
  gSvcStatus.dwWaitHint = dwWaitHint;

  if (dwCurrentState == SERVICE_START_PENDING)
    gSvcStatus.dwControlsAccepted = 0;
  else gSvcStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP;

  if ((dwCurrentState == SERVICE_RUNNING) ||
      (dwCurrentState == SERVICE_STOPPED))
    gSvcStatus.dwCheckPoint = 0;
  else gSvcStatus.dwCheckPoint = dwCheckPoint++;

  // Report the status of the service to the SCM.
  SetServiceStatus(gSvcStatusHandle, &gSvcStatus);
}

VOID WINAPI SvcMain(DWORD dwArgc, LPTSTR *lpszArgv) {
  gSvcStatusHandle = RegisterServiceCtrlHandler(
      SVCNAME,
      SvcCtrlHandler);

  if (!gSvcStatusHandle) {
    SvcReportEvent(TEXT("RegisterServiceCtrlHandler"));
    return;
  }
  gSvcStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
  gSvcStatus.dwServiceSpecificExitCode = 0;

  // Report initial status to the SCM

  ReportSvcStatus(SERVICE_START_PENDING, NO_ERROR, 3000);
  SvcInit(dwArgc, lpszArgv);


}

VOID SvcInit(DWORD dwArgc, LPTSTR *lpszArgv) {
  // TO_DO: Declare and set any required variables.
  //   Be sure to periodically call ReportSvcStatus() with
  //   SERVICE_START_PENDING. If initialization fails, call
  //   ReportSvcStatus with SERVICE_STOPPED.

  // Create an event. The control handler function, SvcCtrlHandler,
  // signals this event when it receives the stop control code.

  ghSvcStopEvent = CreateEvent(
      NULL, // default security attributes
      TRUE, // manual reset event
      FALSE, // not signaled
      NULL); // no name

  if (ghSvcStopEvent == NULL) {
    ReportSvcStatus(SERVICE_STOPPED, NO_ERROR, 0);
    return;
  }

  // Report running status when initialization is complete.
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



  /*
   *
   * Starting Application Services from configuration
   *
   */

  if (string(org::esb::config::Config::getProperty("hive.start")) == "true") {
    string base_path = org::esb::config::Config::getProperty("hive.base_path");

    //    Messenger::getInstance().sendMessage(Message().setProperty("databaseservice", org::esb::hive::START));
    Messenger::getInstance().sendMessage(Message().setProperty("processunitwatcher", org::esb::hive::START));
    //    Messenger::getInstance().sendMessage(Message().setProperty("jobwatcher", org::esb::hive::START));
    Messenger::getInstance().sendMessage(Message().setProperty("hivelistener", org::esb::hive::START));
  }

  if (string(org::esb::config::Config::getProperty("web.start")) == "true" ||
      string(org::esb::config::Config::getProperty("hive.mode")) == "setup") {
    Messenger::getInstance().sendRequest(Message().setProperty("webserver", org::esb::hive::START));
  }

  if (string(org::esb::config::Config::getProperty("hive.autoscan")) == "true") {
    Messenger::getInstance().sendMessage(Message().
        setProperty("directoryscan", org::esb::hive::START).
        setProperty("directory", org::esb::config::Config::getProperty("hive.scandir")).
        setProperty("interval", org::esb::config::Config::getProperty("hive.scaninterval")));
    Messenger::getInstance().sendRequest(Message().setProperty("exportscanner", org::esb::hive::START));
    Messenger::getInstance().sendMessage(Message().setProperty("jobscanner", org::esb::hive::START));
  }
  if (string(org::esb::config::Config::getProperty("mode.client")) == "On") {
    Messenger::getInstance().sendRequest(Message().setProperty("hiveclient", org::esb::hive::START));
  }

  ReportSvcStatus(SERVICE_RUNNING, NO_ERROR, 0);

  // TO_DO: Perform work until service stops.
  while (1) {
    // Check whether to stop the service.

    WaitForSingleObject(ghSvcStopEvent, INFINITE);
    /*
     *
     * Stopping Application Services from configuration
     *
     */
    Messenger::getInstance().sendRequest(Message().setProperty("directoryscan", org::esb::hive::STOP));
    Messenger::getInstance().sendRequest(Message().setProperty("jobscanner", org::esb::hive::STOP));
    Messenger::getInstance().sendRequest(Message().setProperty("jobwatcher", org::esb::hive::STOP));
    Messenger::getInstance().sendRequest(Message().setProperty("processunitwatcher", org::esb::hive::STOP));
    Messenger::getInstance().sendRequest(Message().setProperty("webserver", org::esb::hive::STOP));
    Messenger::getInstance().sendRequest(Message().setProperty("hiveclient", org::esb::hive::STOP));
    Messenger::getInstance().sendRequest(Message().setProperty("databaseservice", org::esb::hive::STOP));
    Messenger::getInstance().sendRequest(Message().setProperty("hivelistener", org::esb::hive::STOP));

    Messenger::free();
    ReportSvcStatus(SERVICE_STOPPED, NO_ERROR, 0);
    return;
  }
}

void start_win32() {
  SERVICE_TABLE_ENTRY DispatchTable[] = {
    { SVCNAME, (LPSERVICE_MAIN_FUNCTION) SvcMain},
    { NULL, NULL}
  };
  if (!StartServiceCtrlDispatcher(DispatchTable)) {
    SvcReportEvent(TEXT("StartServiceCtrlDispatcher"));
  }

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


  string host = org::esb::config::Config::getProperty("client.host", "localhost");
  int port = atoi(org::esb::config::Config::getProperty("client.port", "20200"));

  org::esb::hive::HiveClient client(host, port);
  Messenger::getInstance().addMessageListener(client);

  Messenger::getInstance().sendRequest(Message().setProperty("hiveclient", org::esb::hive::START));

  org::esb::hive::HiveClientAudio clientaudio(host, port);
  Messenger::getInstance().addMessageListener(clientaudio);

  Messenger::getInstance().sendRequest(Message().setProperty("hiveclientaudio", org::esb::hive::START));

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

  if (string(org::esb::config::Config::getProperty("web.start")) == "true" ||
      string(org::esb::config::Config::getProperty("hive.mode")) == "setup") {
    Messenger::getInstance().sendRequest(Message().setProperty("webserver", org::esb::hive::START));
  }

  if (string(org::esb::config::Config::getProperty("hive.autoscan")) == "true") {
    Messenger::getInstance().sendMessage(Message().
        setProperty("directoryscan", org::esb::hive::START).
        setProperty("directory", org::esb::config::Config::getProperty("hive.scandir")).
        setProperty("interval", org::esb::config::Config::getProperty("hive.scaninterval")));
    Messenger::getInstance().sendRequest(Message().setProperty("exportscanner", org::esb::hive::START));
    Messenger::getInstance().sendMessage(Message().setProperty("jobscanner", org::esb::hive::START));

  }
  if (string(org::esb::config::Config::getProperty("mode.client")) == "On") {
    Messenger::getInstance().sendRequest(Message().setProperty("hiveclient", org::esb::hive::START));
    Messenger::getInstance().sendRequest(Message().setProperty("hiveclientaudio", org::esb::hive::START));
  }

  ctrlCHitWait();
  loginfo("shutdown app, this will take some time!");
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

void stop() {
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
  Messenger::getInstance().sendRequest(Message().setProperty("hiveclient", org::esb::hive::STOP));

  Messenger::free();
  //  mysql_library_end();

}

void listener(int argc, char *argv[]) {

  //  Setup::check();
#ifdef WIN32
  if (std::string(Config::getProperty("hive.start_as")) == "daemon") {
    start_win32();
  } else
    start();
#else
  if (std::string(Config::getProperty("hive.start_as")) == "daemon") {
    int i;
    if (getppid() == 1) return; /* already a daemon */
    i = fork();
    if (i < 0) exit(1); /* fork error */
    if (i > 0) exit(0); /* parent exits */
    /* child (daemon) continues */
    setsid(); /* obtain a new process group */
  }
  start();
#endif


  //  org::esb::config::Config::close();
  //  stop();

}

