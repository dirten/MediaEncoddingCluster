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
      foreach(std::string host, hosts){
        StringTokenizer tok(host,":");
        //int port=0;

        if(tok.countTokens()==2){
          std::string hostname=tok.nextToken();
          int port=atoi(tok.nextToken().c_str());
          Ptr<HTTPClientSession> session=new HTTPClientSession(hostname, port);

          _clients.push_back(session);
        }
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
