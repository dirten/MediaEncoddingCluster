#ifndef SIMPLE_H
#define SIMPLE_H
#include "Engine.h"
#include "org/esb/hive/job/ProcessUnit.h"
#include "org/esb/db/hivedb.hpp"

namespace mhivestorage{
  namespace engines {
    class Simple : public Engine
    {
    public:
      Simple( db::HiveDb database, std::string storage_path);
      void put(boost::shared_ptr<org::esb::hive::job::ProcessUnit>unit);
      boost::shared_ptr<org::esb::hive::job::ProcessUnit> get();

      void enque(boost::shared_ptr<org::esb::hive::job::ProcessUnit>unit);
      boost::shared_ptr<org::esb::hive::job::ProcessUnit> deque();

      void commit(std::string uuid);
      void rollback(std::string uuid);

    private:
      db::HiveDb database;
      std::string _storage_path;
    };
    }
  }

#endif // SIMPLE_H
