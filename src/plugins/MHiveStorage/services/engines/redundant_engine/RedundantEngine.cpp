#include "RedundantEngine.h"
#include "org/esb/util/Log.h"
#include "org/esb/util/Foreach.h"
#include "org/esb/util/StringTokenizer.h"

using org::esb::util::StringTokenizer;
using Poco::Net::HTTPClientSession;

namespace mhivestorage{
  namespace engines {

    RedundantEngine::RedundantEngine(db::HiveDb database, std::string storage_path, std::vector<std::string> hosts, int self_port) : Simple(database, storage_path), _mutex(self_port, hosts)
    {
      std::vector<mongo::HostAndPort>rs_hosts;
      foreach(std::string host, hosts){
        rs_hosts.push_back(mongo::HostAndPort(host));
      }
      mongo::DBClientReplicaSet * dbcrs=new mongo::DBClientReplicaSet("first", rs_hosts);
      // c=new mongo::DBClientConnection(true, dbcrs);
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

      /*distributeing the process units*/


    }

    boost::shared_ptr<org::esb::hive::job::ProcessUnit> RedundantEngine::deque()
    {
      //boost::mutex::scoped_lock enqueue_lock(_mutex);
      return Simple::deque();
    }

    void RedundantEngine::commit(std::string uuid)
    {
      Simple::commit(uuid);
    }

    void RedundantEngine::rollback(std::string uuid)
    {
      Simple::rollback(uuid);
    }

    }
  }
