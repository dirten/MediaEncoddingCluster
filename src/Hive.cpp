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
//#include "config.h"
#include "org/esb/config/config.h"
#include "boost/program_options.hpp"
#include "boost/asio.hpp"
#include "org/esb/api/JsonServer.h"
#include "org/esb/hive/job/ProcessUnitController.h"
#include "org/esb/lang/CtrlCHitWaiter.h"

#include "org/esb/hive/DirectoryScanner.h"
#include "org/esb/av/AV.h"
#include "org/esb/av/FormatBaseStream.h"

#include "org/esb/hive/FileImporter.h"
#include "org/esb/hive/FileExporter.h"
#include "org/esb/hive/ExportScanner.h"
#include "org/esb/hive/DatabaseService.h"
#include "org/esb/util/Decimal.h"
#include "org/esb/util/Foreach.h"
#include "org/esb/hive/HiveClient.h"
#include "org/esb/hive/HiveClientAudio.h"
#include "org/esb/hive/HiveListener.h"
#include "org/esb/hive/Setup.h"
#include "org/esb/signal/Messenger.h"
#include "org/esb/signal/Message.h"
#include "org/esb/util/StringUtil.h"
#include "org/esb/core/Application.h"
#include "org/esb/mq/QueueConnection.h"

#if !defined(_WIN32)

#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>
#include <sys/syscall.h>

#else
#include <windows.h>
#include <tchar.h>
#include <strsafe.h>
#include "client/windows/handler/exception_handler.h"
#endif  // !_WIN32
//#include "org/esb/mq/QueueManager.h"
//#include "org/esb/mq/QueueConnection.h"
#include "org/esb/util/LoggerConfig.cpp"
#include "org/esb/util/Log.h"
#include "org/esb/util/LogConfigurator.cpp"


//#include "org/esb/lang/StackDumper.h"
#include "org/esb/hive/NodeResolver.h"
#include "org/esb/hive/CodecFactory.h"
#include "org/esb/lang/Process.h"
#include "org/esb/lang/ProcessException.h"
//#include "org/esb/rpc/Server.h"
//#include "org/esb/rpc/rpc.pb.h"
#include "MHiveConsole.cpp"
#include "org/esb/core/PluginRegistry.h"
/*
#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
 */
#include "boost/uuid/uuid_generators.hpp"
#include "boost/uuid/uuid_io.hpp"
#include "boost/lexical_cast.hpp"
#include "MainApplication.h"

#include "Poco/Environment.h"
#define TO_STRING(s) #s
using namespace org::esb;
using namespace org::esb::net;
using namespace org::esb::io;
//using namespace org::esb::config;
using namespace org::esb::hive;


using namespace org::esb::util;
using namespace org::esb::signal;
//using namespace std;


using namespace std;

namespace po = boost::program_options;

void listener(int argc, char * argv[]);
void client(int argc, char * argv[]);
void shell(int argc, char * argv[]);
void start();
void start_auto(int argc, char * argv[]);
void setupConfig();
bool setupDatabase();
void setupDefaults();
void checkDirs();
int rec = 0;
std::string _hostname;
int _port = 0;
bool quiet = false;

int main(int argc, char * argv[]) {
  //org::esb::core::Application(argc, argv);
  //isatty(0);
  /*setting default path to Program*/
  //log4cplus::BasicConfigurator conf;

  /*
  Poco::Net::HTTPClientSession session("www.google.de",80);
  Poco::Net::HTTPRequest req(Poco::Net::HTTPRequest::HTTP_GET,"/",Poco::Net::HTTPRequest::HTTP_1_1);
  Poco::Net::HTTPResponse res;
  session.sendRequest(req);
  std::istream& t = session.receiveResponse(res);
  std::cout << res.getStatus();
   */

  std::string base_path;
  if(config::Config::get("hive.base_path").length()){
    base_path=config::Config::get("hive.base_path");
  }else{
    org::esb::io::File f(argv[0]);
    base_path = org::esb::io::File(f.getParent()).getParent();
  }
  std::cout << "BasePath="<<base_path<<std::endl;
  //log4cplus::BasicConfigurator::doConfigure();

  config::Config::setProperty("hive.base_path", base_path);
  //log4cplus::BasicConfigurator::doConfigure();
  //  Config::setProperty("hive.base_path", base_path);
  try {
    po::options_description gen("General options");
    unsigned int cpu_count = Process::getCpuCount();
    gen.add_options()
      ("help", "produce this message")
        ("version", "Prints the Version")
      ("plugindir", "which plugin directory to use");
    /*
            ("debug", "switch of the StackDumper and logging goes to the console instead of file")
            ("loglevel", po::value<std::string > ()->default_value("fatal"), "setting the loglevel for this process");**/
    /*
        po::options_description inst("Install options");
        inst.add_options()
                ("hiveport", po::value<int>()->default_value(20200), "on which port will the hive be listen on")
                ("webport", po::value<int>()->default_value(8080), "on which port will the web admin be listen on")
                ;
     */
    po::options_description ser("Server options");
    ser.add_options()
        //("daemon,d", "start the Hive as Daemon Process")
        ("run,r", "start the Hive as Console Process");
    /*
            ("auto,a", "start the Hive as Console Process with automatic Client/Server resolving")
            ("base,b", po::value<std::string > ()->default_value(base_path), "defining a base path")
            ("stop", po::value<int > (), "stopping a Process defined by the process id")
            ;*/

    po::options_description cli("Client options");

    cli.add_options()
        ("client,i", "start the Hive Client");
    /*
            ("host,h", po::value<std::string > ()->default_value("auto"), "Host to connect")
            ("partition", po::value<std::string > ()->default_value("global"), "assigned partition")
            ("port,p", po::value<int>()->default_value(20200), "Port to connect")
            ("count", po::value<int>()->default_value(cpu_count), "Client Processor Count")
            ;*/
    /*
        po::options_description web("Webserver");
        web.add_options()
                ("web,w", "start the Hive Webserver")
                ;
        po::options_description mon("mon");
        mon.add_options()
                ("mon,m", "start the Mongoose Webserver(an alternative web server)")
                ;
        po::options_description queue("Webserver");
        queue.add_options()
                ("queue,q", "start the Hive Queue Server")
                ("test", "test function")
                ;
     */
    po::options_description priv("");
    priv.add_options()
        ("erlang", "")
        ("console,c", "")
        ("quiet", "")
        ("docroot,d",po::value<std::string > (), "webserver document root")
        ;
    po::options_description all("all");
    setupDefaults();
    setupConfig();
    checkDirs();
    //log4cplus::spi::AppenderFactoryRegistry& reg = log4cplus::spi::getAppenderFactoryRegistry();
    //REG_APPENDER (reg, ConsoleAppender);
    log4cplus::PropertyConfigurator config(LOG4CPLUS_TEXT(config::Config::get("hive.config_path") + "/logging.properties"));
    log4cplus::helpers::Properties & props = const_cast<log4cplus::helpers::Properties&> (config.getProperties());
    props.setProperty(LOG4CPLUS_TEXT("appender.MAIN.File"), LOG4CPLUS_TEXT(config::Config::get("log.path") + "/mhive-debug.log"));
    props.setProperty(LOG4CPLUS_TEXT("appender.ERROR.File"), LOG4CPLUS_TEXT(config::Config::get("log.path") + "/mhive-error.log"));
    config.configure();
    org::esb::av::FormatBaseStream::initialize();

    //std::cout << "logpath"<<getenv("log.path")<<std::endl;
    string base_path = org::esb::config::Config::getProperty("hive.base_path");
    //std::cout << "base_path:" << base_path << std::endl;
    //LOGDEBUG("UserDataPath=" << config::Config::get("hive.user_path"))
    //org::esb::util::LogConfigurator * lconfig=new org::esb::util::LogConfigurator();
    //lconfig->configure();
    //LoggerConfig();
    //log4cplus::BasicConfigurator::doConfigure();
    //Log::open(base_path+"/res");
    //LOGDEBUG("configure Log opened");
    //return 0;
    setupDatabase();
    all.add(gen).add(ser).add(cli);

    std::string pluginDir=Poco::Environment::get(std::string("MHIVE_PLUGIN_DIR"),base_path+"/plugins");
    org::esb::core::PluginRegistry::getInstance()->load(pluginDir);

    //std::cout<<"Plugindir:"<<config::Config::getProperty("MHIVE_PLUGIN_DIR");

    foreach(std::list<std::string>::value_type data, org::esb::core::PluginRegistry::getInstance()->getPluginNameList()) {
      org::esb::core::OptionsDescription od = org::esb::core::PluginRegistry::getInstance()->getOptionsDescription(data);
      if (od.options().size() > 0)
        all.add(od);
    }
    priv.add(all);

    po::variables_map vm;
    try {
      po::store(po::parse_command_line(argc, argv, priv), vm);
    } catch (std::exception & ex) {
      //std::cout <<boost::diagnostic_information(ex)<<std::endl;
      std::cout << ex.what() << "!!!" << std::endl << std::endl;
      std::cout << all << std::endl;
      exit(1);
    }

    po::notify(vm);

    foreach(po::variables_map::value_type & val, vm) {
      if (vm[val.first].value().type() == typeid (int)) {
        config::Config::setProperty(val.first, StringUtil::toString(vm[val.first].as<int>()));
      } else if (vm[val.first].value().type() == typeid (double)) {
        config::Config::setProperty(val.first, StringUtil::toString(vm[val.first].as<double>()));
      } else if (vm[val.first].value().type() == typeid (bool)) {
        config::Config::setProperty(val.first, StringUtil::toString(vm[val.first].as<bool>()));
      } else {
        config::Config::setProperty(val.first, vm[val.first].as<std::string > ());
      }
    }
    config::Config::setProperty("partition", "global");
    //config::Config::setProperty("hive.port", StringUtil::toString(vm["hiveport"].as<int> ()));
    //config::Config::setProperty("web.port", StringUtil::toString(vm["webport"].as<int> ()));

    if (vm.count("help") || argc == 1) {
      cout << all << "\n";
      exit(0);
    }
    org::esb::core::PluginRegistry::getInstance()->initPlugins();
    if (vm.count("loglevel")) {
      config::Config::setProperty("loglevel", vm["loglevel"].as<string > ());
    }
    if (!vm.count("quiet")) {
      std::cout << "" << std::endl;
      std::cout << "******************************************************************" << std::endl;
      std::cout << "* MediaEncodingCluster, Copyright (C) 2000-2011   Jan Hoelscher  *" << std::endl;
      std::cout << "*                                                                *" << std::endl;
      std::cout << "* This program is Licensed under the terms in the LICENSE file   *" << std::endl;
      std::cout << "*                                                                *" << std::endl;
      std::cout << "* This program is distributed in the hope that it will be useful,*" << std::endl;
      std::cout << "* but WITHOUT ANY WARRANTY; without even the implied warranty of *" << std::endl;
      std::cout << "* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.           *" << std::endl;
      std::cout << "******************************************************************" << std::endl;
      std::cout << "" << std::endl;
    } else {
      quiet = true;
    }
    //std::cout << "here" << std::endl;
    //config::Config::init("hive.cfg");
    //if (getenv("log.path"))
    //  std::cout << "logpath" << getenv("log.path") << std::endl;
    //else
    //  std::cout << "logpath is null" << std::endl;

    if (vm.count("stop")) {
      if (vm["stop"].as<int> () <= 0) {
        LOGERROR("please provide a Process Id to stop");
        return 1;
      }
      org::esb::lang::Process p(vm["stop"].as<int> ());
      try {
        p.stop();
      } catch (org::esb::lang::ProcessException & ex) {
        LOGERROR("failed stopping process with id: " << vm["stop"].as<int> ());
      }
      try {
        p.kill();
      } catch (org::esb::lang::ProcessException & ex) {
        LOGERROR("failed killing process with id: " << vm["stop"].as<int> ());
      }

      return 0;
    }

    //config::Config::init(config_path);


    //std::string logconfigpath = base_path;
    //logconfigpath.append("/res");


    //if(config::Config::get("debug")!="true")
    //new StackDumper(config::Config::get("hive.dump_path"));


    //av_register_all();
    //avcodec_init();
    //avcodec_register_all();

    if (vm.count("console")) {
      console();


    }
    if (vm.count("erlang")) {
      LOGDEBUG("test option");

      /*
       *
       * Initializing Application Services
       *
       */

      //org::esb::hive::DirectoryScanner dirscan;
      //Messenger::getInstance().addMessageListener(dirscan);

      //org::esb::hive::ExportScanner expscan;
      //Messenger::getInstance().addMessageListener(expscan);


      //  org::esb::web::WebServer webserver;
      //  Messenger::getInstance().addMessageListener(webserver);

      //org::esb::hive::HiveListener hive;
      //Messenger::getInstance().addMessageListener(hive);

      //org::esb::hive::job::ProcessUnitController puw;
      //Messenger::getInstance().addMessageListener(puw);
      //  LOGDEBUG("here")


      /*
       *
       * Starting Application Services from configuration
       *
       */

      //  Messenger::getInstance().sendMessage(Message().setProperty("databaseservice", org::esb::hive::START));

      //  if (string(org::esb::config::Config::getProperty("hive.start")) == "true") {
      //Messenger::getInstance().sendRequest(Message().setProperty("webserver", org::esb::hive::START));
      //string base_path = org::esb::config::Config::getProperty("hive.base_path");
      //Messenger::getInstance().sendMessage(Message().setProperty("processunitcontroller", org::esb::hive::START));
      //    Messenger::getInstance().sendMessage(Message().setProperty("jobwatcher", org::esb::hive::START));
      //Messenger::getInstance().sendMessage(Message().setProperty("hivelistener", org::esb::hive::START));
      //  }

      //  if (string(org::esb::config::Config::getProperty("web.start")) == "true" || string(org::esb::config::Config::getProperty("hive.mode")) == "setup") {
      //  }

      //  if (string(org::esb::config::Config::getProperty("hive.autoscan")) == "true") {
      //  Messenger::getInstance().sendMessage(Message(). setProperty("directoryscan", org::esb::hive::START). setProperty("directory", org::esb::config::Config::getProperty("hive.scandir")). setProperty("interval", org::esb::config::Config::getProperty("hive.scaninterval")));
      //Messenger::getInstance().sendRequest(Message().setProperty("exportscanner", org::esb::hive::START));
      //    Messenger::getInstance().sendMessage(Message().setProperty("jobscanner", org::esb::hive::START));

      //  }
      //org::esb::api::JsonServer server(vm["webport"].as<int> ());

      //  LOGINFO("wait for shutdown!");
      //org::esb::rpc::Server server(6000);
      //boost::thread(boost::bind(&org::esb::rpc::Server::start, &server));

      getc(stdin);
      LOGINFO("shutdown app, this will take some time!");
      /*
       *
       * Stopping Application Services from configuration
       *
       */
      /*
      Messenger::getInstance().sendRequest(Message().setProperty("hiveclient", org::esb::hive::STOP));
      Messenger::getInstance().sendRequest(Message().setProperty("hiveclientaudio", org::esb::hive::STOP));
      Messenger::getInstance().sendRequest(Message().setProperty("jobscanner", org::esb::hive::STOP));
      //  Messenger::getInstance().sendRequest(Message().setProperty("directoryscan", org::esb::hive::STOP));
      Messenger::getInstance().sendRequest(Message().setProperty("exportscanner", org::esb::hive::STOP));
      Messenger::getInstance().sendRequest(Message().setProperty("jobwatcher", org::esb::hive::STOP));
      Messenger::getInstance().sendRequest(Message().setProperty("processunitcontroller", org::esb::hive::STOP));
      Messenger::getInstance().sendRequest(Message().setProperty("webserver", org::esb::hive::STOP));
      Messenger::getInstance().sendRequest(Message().setProperty("databaseservice", org::esb::hive::STOP));
      Messenger::getInstance().sendRequest(Message().setProperty("hivelistener", org::esb::hive::STOP));
       */
      //CodecFactory::free();
      Messenger::free();
    }

    if(vm.count("docroot")){
      config::Config::setProperty("web.docroot", vm["docroot"].as<std::string>());
      std::cout << vm["docroot"].as<std::string>()<<std::endl;
    }

    if (vm.count("run")) {
      LOGDEBUG("start mhive server");
      //org::esb::hive::DatabaseService::start(config::Config::getProperty("hive.base_path"));

      //org::esb::api::JsonServer server(vm["webport"].as<int> ());
      config::Config::setProperty("mode", "server");
      org::esb::core::PluginRegistry::getInstance()->startServerServices();
      Ptr<org::esb::mq::QueueConnection> con=new org::esb::mq::QueueConnection("localhost", 20202);
      if(!con->queueExist("read_q")){
          con->createQueue("read_q");
      }
      if(!con->queueExist("write_q")){
          con->createQueue("write_q");
      }

      //config::Config::setProperty("mode", "client");
      //org::esb::core::PluginRegistry::getInstance()->startClientServices();

      std::string port = config::Config::get("web.port");
      if (!quiet) {
        //std::cout << "mhive server is running, open the url http://localhost:" << port << std::endl;
        std::cout << "Press ctrl & c to stop the program" << std::endl;
      }
      org::esb::lang::CtrlCHitWaiter::wait();
      if (!quiet) {
        std::cout << "\rshutdown app, this will take a minute!" << std::endl;
        ;
      }
      org::esb::core::PluginRegistry::getInstance()->stopServices();

    }

    if (vm.count("client")) {

      log4cplus::helpers::Properties & props = const_cast<log4cplus::helpers::Properties&> (config.getProperties());
      props.setProperty(LOG4CPLUS_TEXT("appender.MAIN.File"), LOG4CPLUS_TEXT(config::Config::get("log.path") + "/mhive-client-debug.log"));
      props.setProperty(LOG4CPLUS_TEXT("appender.ERROR.File"), LOG4CPLUS_TEXT(config::Config::get("log.path") + "/mhive-client-error.log"));
      config.configure();

      config::Config::setProperty("mode", "client");
      org::esb::core::PluginRegistry::getInstance()->startClientServices();
      org::esb::lang::CtrlCHitWaiter::wait();
      org::esb::core::PluginRegistry::getInstance()->stopServices();
      /*
      config::Config::setProperty("client.port", Decimal(vm["port"].as<int> ()).toString().c_str());
      config::Config::setProperty("client.count", Decimal(vm["count"].as<int> ()).toString().c_str());
      config::Config::setProperty("client.host", vm["host"].as<std::string > ().c_str());
      client(argc, argv);*/
    }

    if (vm.count("auto")) {
      LOGDEBUG("start mhive server in auto mode, first node will be startup as server");
      config::Config::setProperty("client.port", Decimal(vm["port"].as<int> ()).toString().c_str());
      config::Config::setProperty("client.host", vm["host"].as<std::string > ().c_str());
      //start_auto(argc, argv);
    }


    if (vm.count("mon")) {

      // Wait until enter is pressed, then exit
      //org::esb::api::JsonServer server(8081);
      getchar();

    }


    if (vm.count("version")) {
      cout << "MediaEncodingCluster 0.0.5.0" << endl;
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
  //  delete[] path;
  //  delete[] base_path;
  org::esb::hive::DatabaseService::stop();
  org::esb::core::PluginRegistry::close();

  org::esb::config::Config::close();
  //CodecFactory::free();
  Messenger::free();
  LOGINFO("MHive is not running anymore!!!")
      //Log::close();

      return 0;
}
#if 0
#include "subsystemone.h"
int main_poco(int argc, char ** argv){
  MainApplication * app=new MainApplication(argc, argv);
  //app->addSubsystem(new SubSystemOne());
  //app->init(argc, argv);
  return app->run(argc,argv);
}
#endif
bool setupDatabase() {
  org::esb::hive::DatabaseService::start(config::Config::getProperty("hive.base_path"));
  if (!DatabaseService::databaseExist()) {
    DatabaseService::createDatabase();
    DatabaseService::createTables();
    DatabaseService::updateTables();
    //DatabaseService::loadPresets();
    {
      db::HiveDb db = org::esb::hive::DatabaseService::getDatabase();
      org::esb::io::File dir(config::Config::get("hive.base_path") + "/presets");
      org::esb::io::FileList presets = dir.listFiles();

      foreach(org::esb::io::FileList::value_type p, presets) {
        org::esb::io::FileInputStream fis(p.get());
        std::string data;
        fis.read(data);
        try {
          JSONNode d = libjson::parse(data);
          boost::uuids::uuid uuid = boost::uuids::random_generator()();
          std::string uuidstr = boost::lexical_cast<std::string > (uuid);
          db::Preset preset(db);
          preset.data = data;
          preset.uuid = uuidstr;
          preset.name = d["name"].as_string();
          preset.update();
        } catch (...) {
        }
      }

      std::map<std::string, std::string> conf;
      conf["hive.mode"] = "server";
      conf["hive.port"] = config::Config::getProperty("hive.port"); //StringUtil::toString(vm["hiveport"].as<int> ());
      conf["web.port"] = config::Config::getProperty("web.port"); //StringUtil::toString(vm["webport"].as<int> ());

      conf["hive.start"] = "true";
      conf["web.start"] = "true";
      conf["hive.autoscan"] = "true";
      conf["hive.scaninterval"] = "30";
      std::string webroot = std::string(config::Config::getProperty("hive.base_path"));
      webroot.append("/web");
      conf["web.docroot"] = webroot;

      std::map<std::string, std::string>::iterator it = conf.begin();
      db.begin();
      for (; it != conf.end(); it++) {
        db::Config cfg(db);
        cfg.configkey = it->first;
        cfg.configval = it->second;
        cfg.update();
        LOGDEBUG("key=" << it->first << " val=" << it->second);
      }
      db.commit();
    }
  } else {
    DatabaseService::updateTables();
  }
  /**stopping the internal database service*/
  org::esb::hive::DatabaseService::stop();
  return true;
}

void checkDirs() {
  org::esb::io::File upath(config::Config::get("hive.user_path"));
  if (!upath.exists())
    upath.mkdir();

  org::esb::io::File dpath(config::Config::get("hive.dump_path"));
  if (!dpath.exists())
    dpath.mkdir();

  org::esb::io::File tpath(config::Config::get("hive.tmp_path"));
  if (!tpath.exists())
    tpath.mkdir();

  org::esb::io::File datadir(config::Config::get("hive.data_path"));
  if (!datadir.exists())
    datadir.mkdir();

  org::esb::io::File confdir(config::Config::get("hive.config_path"));
  if (!confdir.exists())
    confdir.mkdir();

  org::esb::io::File logdir(config::Config::get("log.path"));
  if (!logdir.exists())
    logdir.mkdir();

  org::esb::io::File graphdir(config::Config::get("hive.graph_path"));
  if (!graphdir.exists())
    graphdir.mkdir();

}

void setupDefaults() {
}

void setupConfig() {
  std::string bpath = config::Config::get("hive.base_path");
#ifdef __WIN32__
  std::string upath = config::Config::get("APPDATA") + "/mhive";
#elif defined __APPLE__
  std::string upath = config::Config::get("HOME") + "/.mhive";
#elif defined __LINUX__
  std::string upath = config::Config::get("HOME") + "/.mhive";
#else
  //#error "plattform not supported"
#endif
  config::Config::setProperty("hive.user_path", upath);
  config::Config::setProperty("web.docroot", bpath + "/web");
  config::Config::setProperty("hive.config_path", upath + "/conf");
  config::Config::setProperty("hive.graph_path", upath + "/graphs");
  config::Config::setProperty("hive.dump_path", upath + "/dmp");
  config::Config::setProperty("hive.tmp_path", upath + "/tmp");
  config::Config::setProperty("hive.data_path", upath + "/data");
  config::Config::setProperty("preset.path", bpath + "/presets");
  config::Config::setProperty("log.path", upath + "/logs");
  config::Config::setProperty("db.url", "database=" + upath + "/data/hive.db");
#ifdef __WIN32__
  config::Config::setProperty("PATH", config::Config::get("PATH") + ";" + bpath + "/plugins");
#elif defined __APPLE__
  config::Config::setProperty("DYLD_LIBRARY_PATH", config::Config::get("DYLD_LIBRARY_PATH") + ":" + bpath + "/plugins");
#elif defined __LINUX__
  config::Config::setProperty("LD_LIBRARY_PATH", config::Config::get("LD_LIBRARY_PATH") + ":" + bpath + "/plugins");
#else
  //#error "plattform not supported"
#endif
  org::esb::io::File src_logfile(bpath+"/res/logging.properties");
  org::esb::io::File trg_logfile(upath + "/conf/logging.properties");
  if(src_logfile.exists()){
    if(!trg_logfile.exists()){
      src_logfile.copyTo(trg_logfile);
    }
  }
  //LOGDEBUG("LIBRARY_PATH="<<config::Config::get("DYLD_LIBRARY_PATH"));
  /*
    std::string logpath=std::string("log.path=").append(bpath).append("/logs");
    char * pa=new char[logpath.length()+1];//const_cast<char*>(logpath.c_str());

    memset(pa,0,logpath.length()+1);
    memcpy(pa,logpath.c_str(),logpath.length());
    putenv(pa);*/
  //std::cout << "logpath"<<pa<<std::endl;
  //std::cout << "logpathenv" << getenv("log.path") << std::endl;
  //config::Config::setProperty("authentication", "true");

}
