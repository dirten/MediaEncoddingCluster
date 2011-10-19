/* 
 * File:   Task.h
 * Author: HoelscJ
 *
 * Created on 19. Oktober 2011, 10:23
 */

#ifndef TASK_H
#define	TASK_H
#include "Plugin.h"
#include "PluginRegistry.h"
#include "org/esb/lang/Ptr.h"
#include "exports.h"
namespace org {
  namespace esb {
    namespace core {

      class CORE_EXPORT Task : public Plugin {
      public:
        Task();
        virtual ~Task();
        virtual void prepare();
        virtual void execute();
      private:

      };
    }
  }
}
#define REGISTER_TASK(name,instance) \
class RegisterTaskFactory##instance : public org::esb::core::TaskFactory { \
        public: \
                RegisterTaskFactory##instance() { \
                        std::string tmpname=#instance; \
                        org::esb::core::PluginRegistry::getInstance()->registerTaskFactory(tmpname, this); \
	        } \
	        ~RegisterTaskFactory##instance() { } \
                Ptr<org::esb::core::Task> create() { \
                        return Ptr<org::esb::core::Task>(new instance()); \
                } \
                org::esb::core::OptionsDescription getOptionsDescription() { \
                return ##instance::getOptionsDescription(); \
                } \
} RegisterTask##instance##Instance; 



#endif	/* TASK_H */

