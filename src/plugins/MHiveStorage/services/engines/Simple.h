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
      boost::shared_ptr<std::istream>dequeStream();


      void putUnit(org::esb::model::Unit unit);
      void getUnit(org::esb::model::Unit unit);
      void dequeUnit(org::esb::model::Unit unit);

      void putJob(org::esb::model::Job job);
      void getJob(org::esb::model::Job job);

      void putOutputFile(org::esb::model::OutputFile file);
      void getOutputFile(org::esb::model::OutputFile file);

      void commit(std::string uuid);
      void rollback(std::string uuid);

    private:
      db::HiveDb database;
      std::string _storage_path;
    };
    }
  }

#endif // SIMPLE_H
