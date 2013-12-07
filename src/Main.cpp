/*----------------------------------------------------------------------
 *  File    : Main.cpp
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
#include "org/esb/av/AV.h"
#include "org/esb/av/FormatBaseStream.h"

#include "boost/program_options.hpp"
#include "boost/asio.hpp"

#include "org/esb/core/PluginRegistry.h"

#include "org/esb/hive/DatabaseService.h"
#include "org/esb/hive/Environment.h"

#include "org/esb/io/FileInputStream.h"

#include "org/esb/lang/CtrlCHitWaiter.h"

#include "org/esb/libjson/JSONNode.h"
#include "org/esb/libjson/libjson.h"

#include "org/esb/signal/Messenger.h"
#include "org/esb/signal/Message.h"

#include "org/esb/util/Foreach.h"
#include "org/esb/util/Log.h"
#include "org/esb/util/StringUtil.h"
#include "org/esb/util/UUID.h"

using org::esb::hive::Environment;
using org::esb::signal::Messenger;
using org::esb::util::StringUtil;

namespace po = boost::program_options;


int main(int argc, char * argv[]) {
  org::esb::hive::Environment::build(argc, argv);
  org::esb::av::FormatBaseStream::initialize();
  try {
    po::options_description gen("General options");
    gen.add_options()
    ("help", "produce this message")
    ("version", "Prints the Version");

    po::options_description ser("Server options");
    ser.add_options()
    ("run,r", "start the Hive as Console Process");

    po::options_description cli("Client options");

    cli.add_options()
    ("client,i", "start the Hive Client");

    po::options_description priv("");
    priv.add_options()
    ("erlang", "")
    ("console,c", "")
    ("quiet", "")
    ("docroot,d", po::value<std::string > (), "webserver document root");
    po::options_description all("all");

    log4cplus::PropertyConfigurator config(LOG4CPLUS_TEXT(Environment::get("hive.config_path") + "/logging.properties"));
    log4cplus::helpers::Properties & props = const_cast<log4cplus::helpers::Properties&> (config.getProperties());
    props.setProperty(LOG4CPLUS_TEXT("appender.MAIN.File"), LOG4CPLUS_TEXT(Environment::get("log.path") + "/mhive-debug.log"));
    props.setProperty(LOG4CPLUS_TEXT("appender.ERROR.File"), LOG4CPLUS_TEXT(Environment::get("log.path") + "/mhive-error.log"));
    config.configure();

    string base_path = Environment::get("hive.base_path");

    all.add(gen).add(ser).add(cli);

    std::string pluginDir = Environment::get("MHIVE_PLUGIN_DIR", base_path+"/plugins");
    org::esb::core::PluginRegistry::getInstance()->load(pluginDir);


    foreach(std::list<std::string>::value_type data, org::esb::core::PluginRegistry::getInstance()->getPluginNameList()) {
      org::esb::core::OptionsDescription od = org::esb::core::PluginRegistry::getInstance()->getOptionsDescription(data);
      if (od.options().size() > 0)
        all.add(od);
    }
    priv.add(all);

    po::variables_map vm;
    try {
      po::store(po::parse_command_line(argc, argv, priv), vm);
    } catch(std::exception & ex) {
      // std::cout <<boost::diagnostic_information(ex)<<std::endl;
      std::cout << ex.what() << "!!!" << std::endl << std::endl;
      std::cout << all << std::endl;
      exit(1);
    }

    po::notify(vm);

    foreach(po::variables_map::value_type & val, vm) {
      if (vm[val.first].value().type() == typeid (int)) {
        Environment::set(val.first, StringUtil::toString(vm[val.first].as<int>()));
      } else if (vm[val.first].value().type() == typeid (double)) {
        Environment::set(val.first, StringUtil::toString(vm[val.first].as<double>()));
      } else if (vm[val.first].value().type() == typeid (bool)) {
        Environment::set(val.first, StringUtil::toString(vm[val.first].as<bool>()));
      } else {
        Environment::set(val.first, vm[val.first].as<std::string > ());
      }
    }
    Environment::set("partition", "global");

    if (vm.count("help") || argc == 1) {
      cout << all << "\n";
      exit(0);
    }

    org::esb::core::PluginRegistry::getInstance()->initPlugins();
    if (vm.count("loglevel")) {
      Environment::set("loglevel", vm["loglevel"].as<std::string> ());
    }

    if (!vm.count("quiet")) {
      std::cout << "" << std::endl;
      std::cout << "******************************************************************" << std::endl;
      std::cout << "* MediaEncodingCluster, Copyright (C) 2000-2014   Jan Hoelscher  *" << std::endl;
      std::cout << "*                                                                *" << std::endl;
      std::cout << "* This program is Licensed under the terms in the LICENSE file   *" << std::endl;
      std::cout << "*                                                                *" << std::endl;
      std::cout << "* This program is distributed in the hope that it will be useful,*" << std::endl;
      std::cout << "* but WITHOUT ANY WARRANTY; without even the implied warranty of *" << std::endl;
      std::cout << "* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.           *" << std::endl;
      std::cout << "******************************************************************" << std::endl;
      std::cout << "" << std::endl;
    }

    if(vm.count("docroot")){
      Environment::set("web.docroot", vm["docroot"].as<std::string>());
      std::cout << vm["docroot"].as<std::string>()<<std::endl;
    }

    if (vm.count("run")) {
      LOGDEBUG("start mhive server");

      Environment::set("mode", "server");

      org::esb::core::PluginRegistry::getInstance()->startServerServices();

      std::cout << "Press ctrl & c to stop the program" << std::endl;
      org::esb::lang::CtrlCHitWaiter::wait();
      std::cout << "\rshutdown app, this will take a minute!" << std::endl;
      org::esb::core::PluginRegistry::getInstance()->stopServices();

    }

    if (vm.count("client")) {

      log4cplus::helpers::Properties & props = const_cast<log4cplus::helpers::Properties&> (config.getProperties());
      props.setProperty(LOG4CPLUS_TEXT("appender.MAIN.File"), LOG4CPLUS_TEXT(Environment::get("log.path") + "/mhive-client-debug.log"));
      props.setProperty(LOG4CPLUS_TEXT("appender.ERROR.File"), LOG4CPLUS_TEXT(Environment::get("log.path") + "/mhive-client-error.log"));
      config.configure();

      Environment::set("mode", "client");
      org::esb::core::PluginRegistry::getInstance()->startClientServices();
      org::esb::lang::CtrlCHitWaiter::wait();
      org::esb::core::PluginRegistry::getInstance()->stopServices();

    }

    if (vm.count("version")) {
      cout << "MediaEncodingCluster 0.0.5.0" << endl;
      cout << LIBAVCODEC_IDENT << endl;
      cout << LIBAVFORMAT_IDENT << endl;
      cout << LIBAVUTIL_IDENT << endl;
      cout << LIBSWSCALE_IDENT << endl;
      exit(0);
    }
  } catch (Exception * e) {
    std::cerr << "error: " << e->what() << "\n";
    return -1;
  }

  org::esb::hive::DatabaseService::stop();
  org::esb::core::PluginRegistry::close();

  //org::esb::config::Config::close();
  //CodecFactory::free();
  Messenger::free();
  LOGINFO("MHive is not running anymore!!!")
  //Log::close();

  return 0;

}
