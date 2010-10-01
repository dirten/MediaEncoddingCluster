
#include "config.h"
#include "org/esb/io/File.h"

#include "org/esb/lang/CtrlCHitWaiter.h"
#include "org/esb/hive/DatabaseService.h"
#include "org/esb/util/Log.h"
#include "org/esb/config/config.h"
#include "org/esb/web/WebServer.h"
#include "org/esb/signal/Messenger.h"
#include "org/esb/signal/Message.h"
#include "org/esb/hive/job/ProcessUnitController.h"
#include "org/esb/hive/ExportScanner.h"
#include "org/esb/hive/CodecFactory.h"
#include "org/esb/hive/NodeResolver.h"
#include "org/esb/hive/HiveListener.h"
#include "org/esb/av/FormatBaseStream.h"

int main(int argc, char** argv) {
  Log::open("");
    /*initialise the config class*/
  org::esb::config::Config::init("");

  org::esb::hive::Node node;
  node.setData("type", "server");
  node.setData("version", MHIVE_VERSION);
  node.setData("port", org::esb::config::Config::getProperty("client.port", "20200"));
  org::esb::hive::NodeResolver res(boost::asio::ip::address::from_string("0.0.0.0"), boost::asio::ip::address::from_string("239.255.0.1"), 6000, node);
  res.start();

  org::esb::av::FormatBaseStream::initialize();
  /**setting internal db url*/
//  org::esb::config::Config::setProperty("db.url", "host=localhost;database=hive;user=root;port=3306");
  /**starting the internal database service*/
  if (!org::esb::hive::DatabaseService::databaseExist()) {
    org::esb::hive::DatabaseService::createDatabase();
    org::esb::hive::DatabaseService::createTables();
    org::esb::hive::DatabaseService::loadPresets();
  }
  org::esb::hive::DatabaseService::updateTables();
  
  if (!org::esb::config::Config::init("")) {
    LOGERROR("could not load config from Database, exiting!!!");
    exit(1);
  }

  org::esb::web::WebServer webserver;
  org::esb::signal::Messenger::getInstance().addMessageListener(webserver);

  org::esb::hive::job::ProcessUnitController puw;
  org::esb::signal::Messenger::getInstance().addMessageListener(puw);

  org::esb::hive::ExportScanner expscan;
  org::esb::signal::Messenger::getInstance().addMessageListener(expscan);

  org::esb::hive::HiveListener hive;
  org::esb::signal::Messenger::getInstance().addMessageListener(hive);

  org::esb::signal::Messenger::getInstance().sendMessage(org::esb::signal::Message().setProperty("processunitcontroller", org::esb::hive::START));

  org::esb::signal::Messenger::getInstance().sendRequest(org::esb::signal::Message().setProperty("webserver", org::esb::hive::START));

  org::esb::signal::Messenger::getInstance().sendRequest(org::esb::signal::Message().setProperty("exportscanner", org::esb::hive::START));

  org::esb::signal::Messenger::getInstance().sendMessage(org::esb::signal::Message().setProperty("hivelistener", org::esb::hive::START));

  org::esb::lang::CtrlCHitWaiter::wait();
  LOGINFO("shutdown app, this will take some time!");
  /*
   *
   * Stopping Application Services from configuration
   *
   */
  org::esb::signal::Messenger::getInstance().sendRequest(org::esb::signal::Message().setProperty("exportscanner", org::esb::hive::STOP));
  org::esb::signal::Messenger::getInstance().sendRequest(org::esb::signal::Message().setProperty("processunitcontroller", org::esb::hive::STOP));
  org::esb::signal::Messenger::getInstance().sendRequest(org::esb::signal::Message().setProperty("webserver", org::esb::hive::STOP));
  org::esb::hive::CodecFactory::free();
    org::esb::signal::Messenger::getInstance().sendRequest(org::esb::signal::Message().setProperty("hivelistener", org::esb::hive::STOP));

//  org::esb::hive::DatabaseService::stop();
  org::esb::signal::Messenger::free();

}