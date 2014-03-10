#include "RedundantEngine.h"
#include "org/esb/util/Log.h"
#include "org/esb/util/Foreach.h"

namespace mhivestorage{
  namespace engines {

    RedundantEngine::RedundantEngine(boost::shared_ptr<db::HiveDb> database, std::string storage_path, std::vector<std::string> hosts, int self_port) : Simple(database, storage_path), _mutex(self_port, hosts)
    {
      foreach(std::string host, hosts){
        LOGDEBUG("try using host for redundant group:"<<host);
      }
    }

    void RedundantEngine::put(boost::shared_ptr<org::esb::hive::job::ProcessUnit>unit)
    {
      Simple::put(unit);
    }

    boost::shared_ptr<org::esb::hive::job::ProcessUnit> RedundantEngine::get()
    {
      return Simple::get();
    }

    void RedundantEngine::enque(boost::shared_ptr<org::esb::hive::job::ProcessUnit>unit)
    {
      Simple::enque(unit);
    }

    boost::shared_ptr<org::esb::hive::job::ProcessUnit> RedundantEngine::deque()
    {
      boost::mutex::scoped_lock enqueue_lock(_mutex);
      return Simple::deque();
    }
    }
  }
