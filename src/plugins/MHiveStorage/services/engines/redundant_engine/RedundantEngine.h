#ifndef REDUNTANTENGINE_H
#define REDUNTANTENGINE_H
//#include "org/esb/core/StorageEngine.h"
#include "../Simple.h"
#include "org/esb/hive/job/ProcessUnit.h"
#include "org/esb/db/hivedb.hpp"
#include "DLMBoostMutex.h"
#include "boost/thread/mutex.hpp"
#include "mongo/client/dbclient.h"


#include "Poco/Net/HTTPClientSession.h"

using Poco::Net::HTTPClientSession;
namespace mhivestorage{
  namespace engines {

    class RedundantEngine : public Simple
    {
    public:
      RedundantEngine();
      RedundantEngine( db::HiveDb database, std::string storage_path, std::vector<std::string> hosts, int self_port);
      void startup();
      void put(boost::shared_ptr<org::esb::hive::job::ProcessUnit>unit);
      boost::shared_ptr<org::esb::hive::job::ProcessUnit> get();

      void enque(boost::shared_ptr<org::esb::hive::job::ProcessUnit>unit);
      boost::shared_ptr<org::esb::hive::job::ProcessUnit> deque();

      void commit(std::string uuid);
      void rollback(std::string uuid);
      org::esb::core::OptionsDescription getOptionsDescription();
    private:
      void initRedundant();
      void initSimple();
      //db::HiveDb database;
      std::string _storage_path;
      //DLMBoostMutex _mutex;

      //std::list<Ptr<HTTPClientSession> > _clients;
      Ptr<mongo::DBClientConnection> c;
      Ptr<mongo::DBClientBase> dbcrs;
      Ptr<mongo::GridFS> gridfs;
      boost::mutex enqueue_mutex;



    };
    }
  }

#endif // REDUNTANTENGINE_H
