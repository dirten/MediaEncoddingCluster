#ifndef REDUNTANTENGINE_H
#define REDUNTANTENGINE_H
#include "../Engine.h"
#include "../Simple.h"
#include "org/esb/hive/job/ProcessUnit.h"
#include "org/esb/db/hivedb.hpp"

namespace mhivestorage{
  namespace engines {

    class RedundantEngine : public Simple
    {
    public:
      RedundantEngine( boost::shared_ptr<db::HiveDb> database, std::string storage_path, std::vector<std::string> hosts);

      void put(boost::shared_ptr<org::esb::hive::job::ProcessUnit>unit);
      boost::shared_ptr<org::esb::hive::job::ProcessUnit> get();

      void enque(boost::shared_ptr<org::esb::hive::job::ProcessUnit>unit);
      boost::shared_ptr<org::esb::hive::job::ProcessUnit> deque();

    private:
      boost::shared_ptr<db::HiveDb> database;
      std::string _storage_path;
    };
    }
  }

#endif // REDUNTANTENGINE_H