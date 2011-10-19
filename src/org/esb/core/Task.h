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
	class RegisterTask##instance \
	        { \
			instance * element##instance; \
	                public: \
	                        RegisterTask##instance() \
	                        { \
				element##instance=new instance(); \
                                std::string tmpname=#instance; \
                                org::esb::core::PluginRegistry::getInstance()->registerTask(tmpname, element##instance); \
	                        } \
	                        ~RegisterTask##instance() \
	                        { \
				delete element##instance; \
	                        } \
	        } RegisterTask##instance##Instance##prio; 



#endif	/* TASK_H */

