#ifndef ENGINE_H
#define ENGINE_H
#include "org/esb/hive/job/ProcessUnit.h"
#include "org/esb/db/Job.h"
#include "org/esb/db/OutputFile.h"
#include "org/esb/db/Unit.h"
namespace mhivestorage{
  namespace engines {
    class Engine {
      public:
        virtual void init(){}
        virtual void put(boost::shared_ptr<org::esb::hive::job::ProcessUnit>unit)=0;

        virtual void commit(std::string uuid)=0;
        virtual void rollback(std::string uuid)=0;

        virtual boost::shared_ptr<org::esb::hive::job::ProcessUnit> get()=0;

        virtual void enque(boost::shared_ptr<org::esb::hive::job::ProcessUnit>unit)=0;
        virtual boost::shared_ptr<org::esb::hive::job::ProcessUnit> deque()=0;
        virtual boost::shared_ptr<std::istream>dequeStream()=0;


        virtual void putUnit(org::esb::model::Unit unit)=0;
        virtual void getUnit(org::esb::model::Unit unit)=0;
        virtual void dequeUnit(org::esb::model::Unit unit)=0;

        virtual void putJob(org::esb::model::Job job)=0;
        virtual void putOutputFile(org::esb::model::OutputFile file)=0;

        virtual void getJob(org::esb::model::Job job)=0;
        virtual void getOutputFile(org::esb::model::OutputFile file)=0;

        virtual ~Engine(){}
    };
  }
}
#endif // ENGINE_H
