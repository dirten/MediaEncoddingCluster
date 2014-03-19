#ifndef REDUNTANTENGINE_H
#define REDUNTANTENGINE_H
#include "../Engine.h"
#include "../Simple.h"
#include "org/esb/hive/job/ProcessUnit.h"
#include "org/esb/db/hivedb.hpp"
#include "DLMBoostMutex.h"
#include "boost/bind.hpp"
#include "mongo/client/dbclient.h"


#include "Poco/Net/HTTPClientSession.h"

using Poco::Net::HTTPClientSession;
namespace mhivestorage{
  namespace engines {

    class RedundantEngine : public Simple
    {
    public:
      RedundantEngine( db::HiveDb database, std::string storage_path, std::vector<std::string> hosts, int self_port);

      void put(boost::shared_ptr<org::esb::hive::job::ProcessUnit>unit);
      boost::shared_ptr<org::esb::hive::job::ProcessUnit> get();

      void enque(boost::shared_ptr<org::esb::hive::job::ProcessUnit>unit);
      boost::shared_ptr<org::esb::hive::job::ProcessUnit> deque();

      void commit(std::string uuid);
      void rollback(std::string uuid);

    private:
      //db::HiveDb database;
      std::string _storage_path;
      //DLMBoostMutex _mutex;

      //std::list<Ptr<HTTPClientSession> > _clients;
      Ptr<mongo::DBClientConnection> c;
    };
    }
  }

#endif // REDUNTANTENGINE_H
