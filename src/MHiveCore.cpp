
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
#include "org/esb/av/FormatBaseStream.h"

int main(int argc, char** argv) {
  Log::open("");
  org::esb::io::File base("..");
  LOGDEBUG("BasePath=" << base.getPath());
  string base_path = base.getPath();
  org::esb::av::FormatBaseStream::initialize();
  org::esb::config::Config::setProperty("hive.base_path", base_path.c_str());
  /**setting internal db url*/
  org::esb::config::Config::setProperty("db.url", "host=localhost;database=hive;user=root;port=3306");
  /**starting the internal database service*/
  org::esb::hive::DatabaseService::start(base.getPath());
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
  org::esb::signal::Messenger::getInstance().sendMessage(org::esb::signal::Message().setProperty("processunitcontroller", org::esb::hive::START));

  org::esb::signal::Messenger::getInstance().sendRequest(org::esb::signal::Message().setProperty("webserver", org::esb::hive::START));

  org::esb::signal::Messenger::getInstance().sendRequest(org::esb::signal::Message().setProperty("exportscanner", org::esb::hive::START));

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
  org::esb::hive::DatabaseService::stop();
  org::esb::signal::Messenger::free();

}