#include "engines/redundant_engine/RedundantEngine.h"
#include "StorageService.h"
#include "engines/Simple.h"
#include "org/esb/core/PluginContext.h"
#include "org/esb/signal/Messenger.h"
#include "org/esb/hive/job/ProcessUnit.h"
#include "org/esb/hive/Environment.h"

#include "org/esb/db/Job.h"
#include "org/esb/db/OutputFile.h"
using org::esb::hive::Environment;
namespace mhivestorage{


  StorageService::StorageService()
  {

  }

  void StorageService::onMessage(org::esb::signal::Message &msg)
  {
    if(msg.containsProperty("processunit_get")){
      LOGDEBUG("get received message");
      boost::shared_ptr<org::esb::hive::job::ProcessUnit>unit=_storageEngine->get();
      msg.setProperty("processunit_get", unit);
    }else if(msg.containsProperty("processunit_put")){
      LOGDEBUG("put received message");
      boost::shared_ptr<org::esb::hive::job::ProcessUnit>unit = msg.getProperty<boost::shared_ptr<org::esb::hive::job::ProcessUnit> >("processunit_put");
      _storageEngine->put(unit);
    }else if(msg.containsProperty("processunit_enque")){
      LOGDEBUG("enque received message");
      boost::shared_ptr<org::esb::hive::job::ProcessUnit>unit = msg.getProperty<boost::shared_ptr<org::esb::hive::job::ProcessUnit> >("processunit_enque");
      _storageEngine->enque(unit);
    }else if(msg.containsProperty("processunit_deque")){
      LOGDEBUG("deque received message");
      boost::shared_ptr<org::esb::hive::job::ProcessUnit>unit=_storageEngine->deque();
      msg.setProperty("processunit_deque", unit);
    }else if(msg.containsProperty("processunit_rollback")){
      _storageEngine->rollback(msg.getProperty<std::string>("processunit_rollback"));
    }else if(msg.containsProperty("outputfile.put")){
    }else if(msg.containsProperty("outputfile.get")){

    }else if(msg.containsProperty("job.put")){
      org::esb::model::Job job=msg.getProperty<org::esb::model::Job >("outputfile.put");
    }else if(msg.containsProperty("job.get")){

    }
  }

  void StorageService::startService()
  {
    std::string engine_name=getContext()->getEnvironment<string>("mhivestorage.engine");

    if (engine_name=="simple") {
      _storageEngine=new engines::Simple(getContext()->database, Environment::get("hive.data_path"));
    }else if(engine_name=="redundant"){
      std::vector<std::string> hosts=getContext()->getProperty< std::vector<std::string> >("mhivestorage.hosts");
      //int port=getContext()->getProperty<int>("mhivestorage.port");
      _storageEngine=new engines::RedundantEngine(getContext()->database, Environment::get("hive.data_path"), hosts, 0);
    }else {
      LOGWARN("no storage engine\""<<engine_name<<"\" found!");
      //_storageEngine=new engines::Simple(getContext()->database, Environment::get("hive.data_path"));
    }
    if(_storageEngine){
      _storageEngine->init();
      org::esb::signal::Messenger::getInstance().addMessageListener(*this);
    }
  }

  void StorageService::stopService()
  {
    if(_storageEngine){
      org::esb::signal::Messenger::getInstance().removeMessageListener(*this);
      _storageEngine.reset();
    }
  }

  org::esb::core::OptionsDescription StorageService::getOptionsDescription()
  {
    org::esb::core::OptionsDescription result("mhivestorage");
    result.add_options()
        ("mhivestorage.engine", boost::program_options::value<string >()->default_value("simple"), "which storage engine to use(simple,redundant)")
    ("mhivestorage.hosts", boost::program_options::value< std::vector<string> >()->multitoken(), "ip:port tuples for the other redundant storage")
    ("mhivestorage.port", boost::program_options::value< int >()->default_value(20202), "port number for own redundant storage")
    //("mhivestorage.redundancy_level", boost::program_options::value<int >()->default_value(2), "which redundancy level should the storage engine \"redundant\" use")
    ;
    return result;
  }
  REGISTER_SERVICE("storageservice", StorageService)

}
