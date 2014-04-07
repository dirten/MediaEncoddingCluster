#ifndef SIMPLE_H
#define SIMPLE_H
#include "org/esb/core/StorageEngine.h"
#include "org/esb/hive/job/ProcessUnit.h"
#include "org/esb/db/hivedb.hpp"

namespace mhivestorage{
  namespace engines {
    class Simple : public org::esb::core::StorageEngine
    {
      classlogger("plugins.storage.simple")
    public:
      Simple();
      Simple( db::HiveDb database, std::string storage_path);

      void put(boost::shared_ptr<org::esb::hive::job::ProcessUnit>unit);
      boost::shared_ptr<org::esb::hive::job::ProcessUnit> get();

      void enque(boost::shared_ptr<org::esb::hive::job::ProcessUnit>unit);
      boost::shared_ptr<org::esb::hive::job::ProcessUnit> deque();
      boost::shared_ptr<std::istream>dequeStream();


      void putUnit(org::esb::model::Unit & unit);
      org::esb::model::Unit getUnit(org::esb::model::Unit & unit);

      void writeUnitStream(org::esb::model::Unit & unit,std::ostream& );
      void readUnitStream(org::esb::model::Unit & unit,std::istream& );

      void dequeUnit(org::esb::model::Unit & unit);

      void putJob(org::esb::model::Job & job);
      void getJob(org::esb::model::Job & job);

      void putOutputFile(org::esb::model::OutputFile & file);
      void getOutputFile(org::esb::model::OutputFile & file);
      org::esb::model::OutputFile getOutputFileByUUID(std::string & uuid);
      std::list<org::esb::model::OutputFile> getOutputFileList();

      void putProfile(org::esb::model::Profile & profile);
      void getProfile(org::esb::model::Profile & profile);

      org::esb::model::Profile getProfileByUUID(std::string & uuid);
      std::list<org::esb::model::Profile> getProfileList();
      bool deleteProfile(org::esb::model::Profile & profile);

      void startup();
      void shutdown();

      void commit(std::string uuid);
      void rollback(std::string uuid);

    private:
      db::HiveDb database;
      std::string _storage_path;
    };
    }
  }

#endif // SIMPLE_H
