#ifndef ENGINE_H
#define ENGINE_H
#include "org/esb/hive/job/ProcessUnit.h"

namespace mhivestorage{
  namespace engines {
    class Engine {
      public:
        virtual void init(){}
        virtual void put(boost::shared_ptr<org::esb::hive::job::ProcessUnit>unit)=0;
        virtual boost::shared_ptr<org::esb::hive::job::ProcessUnit> get()=0;

        virtual void enque(boost::shared_ptr<org::esb::hive::job::ProcessUnit>unit)=0;
        virtual boost::shared_ptr<org::esb::hive::job::ProcessUnit> deque()=0;
        virtual ~Engine(){}
    };
  }
}
#endif // ENGINE_H
