#include "StorageService.h"
#include "org/esb/signal/Messenger.h"
#include "org/esb/hive/job/ProcessUnit.h"
namespace mhivestorage{

  StorageService::StorageService()
  {

  }

  void StorageService::onMessage(org::esb::signal::Message &msg)
  {
    if(msg.containsProperty("processunit_pushed")){
      LOGDEBUG("received message");
      boost::shared_ptr<org::esb::hive::job::ProcessUnit>unit = msg.getProperty<boost::shared_ptr<org::esb::hive::job::ProcessUnit> >("pu");
      LOGDEBUG("PU from msg:"<< unit->uuid);
    }else if(msg.containsProperty("processunit_get")){
      LOGDEBUG("received message");
    }else if(msg.containsProperty("processunit_put")){
      LOGDEBUG("received message");
    }
  }

  void StorageService::startService()
  {
    org::esb::signal::Messenger::getInstance().addMessageListener(*this);
  }

  void StorageService::stopService()
  {
    org::esb::signal::Messenger::getInstance().removeMessageListener(*this);
  }

  org::esb::core::OptionsDescription StorageService::getOptionsDescription()
  {
    org::esb::core::OptionsDescription result("mhivestorage");
    return result;
  }
  REGISTER_SERVICE("storageservice", StorageService)

}
