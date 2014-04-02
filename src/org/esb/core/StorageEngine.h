#ifndef ENGINE_H
#define ENGINE_H
#include "org/esb/hive/job/ProcessUnit.h"
#include "org/esb/db/Job.h"
#include "org/esb/db/Profile.h"
#include "org/esb/db/OutputFile.h"
#include "org/esb/db/Unit.h"
#include "PluginRegistry.h"
#include "Plugin.h"

#include "org/esb/lang/Deprecated.h"
namespace org{
  namespace esb {
    namespace core {

      class StorageEngine: public org::esb::core::Plugin {
      public:
        /*function for handling the processunits*/
        virtual void putUnit(org::esb::model::Unit & unit)=0;
        virtual org::esb::model::Unit getUnit(org::esb::model::Unit & unit)=0;
        virtual void dequeUnit(org::esb::model::Unit & unit)=0;

        virtual void saveUnitStream(org::esb::model::Unit & unit,std::ostream&)=0;
        virtual void readUnitStream(org::esb::model::Unit & unit,std::istream &)=0;

        /*functions for handling the jobs*/
        virtual void putJob(org::esb::model::Job & job)=0;
        virtual void getJob(org::esb::model::Job & job)=0;

        /*functions for handling the outputfiles*/
        virtual void putOutputFile(org::esb::model::OutputFile & file)=0;
        virtual void getOutputFile(org::esb::model::OutputFile & file)=0;

        /*functions for handling the profiles*/
        virtual void putProfile(org::esb::model::Profile & profile)=0;
        virtual void getProfile(org::esb::model::Profile & profile)=0;

        /*management functions*/
        virtual void startup()=0;
        virtual void shutdown()=0;

        virtual ~StorageEngine(){}


        /* here are comming the callback functions,
         * @TODO: should i really need this
         */
        boost::function<void(org::esb::model::Unit unit)> processedUnitReceived;


        /*the following functions are deprecated*/
        DEPRECATED(virtual void init(){})
        DEPRECATED(virtual void put(boost::shared_ptr<org::esb::hive::job::ProcessUnit>unit))=0;

        DEPRECATED(virtual void commit(std::string uuid))=0;
        DEPRECATED(virtual void rollback(std::string uuid))=0;

        DEPRECATED(virtual boost::shared_ptr<org::esb::hive::job::ProcessUnit> get())=0;

        DEPRECATED(virtual void enque(boost::shared_ptr<org::esb::hive::job::ProcessUnit>unit))=0;
        DEPRECATED(virtual boost::shared_ptr<org::esb::hive::job::ProcessUnit> deque())=0;
        DEPRECATED(virtual boost::shared_ptr<std::istream>dequeStream())=0;


      };

      }
    }
  }

#define REGISTER_STORAGE(name,type) \
  class Register##type \
          { \
                        org::esb::core::StorageEngine* element##type; \
                  public: \
                          Register##type() \
                          { \
                                element##type=new type(); \
                          org::esb::core::PluginRegistry::getInstance()->registerStorage(std::string(name), element##type); \
                          } \
                                ~Register##type(){ \
                                delete element##type; \
                                } \
          } Register##type##Instance;

#endif // ENGINE_H
