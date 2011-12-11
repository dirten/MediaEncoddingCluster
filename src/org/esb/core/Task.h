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
#ifdef ERROR
#undef ERROR
#endif
namespace org {
  namespace esb {
    namespace core {

      class CORE_EXPORT Task : public Plugin {
      public:
        enum STATUS {
          NONE,
          PREPARE,
          EXECUTE,
          CLEANUP,
          DONE,
          INTERRUPT,
          INTERRUPTED,
          ERROR
        };

      protected:
        void setProgressLength(unsigned int);
        unsigned int getProgressLength();
        void setProgress(int);
        void setStatus(STATUS);
        void setStatusMessage(std::string);

      public:
        Task();
        virtual ~Task();
        virtual void prepare();
        virtual void execute();
        virtual void cleanup();
        virtual int getProgress();
        virtual void interrupt();
        virtual STATUS getStatus();
        virtual std::string getStatusMessage();
        /**in case of tasks, these are file-source/sink*/
        virtual std::string getSource();
        virtual std::string getSink();
      private:
        unsigned int _progress;
        unsigned int _progress_length;
        STATUS _status;
        std::string _status_message;
        std::string _source;
        std::string _sink;
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
                Ptr<org::esb::core::Task> t = Ptr<org::esb::core::Task>(new instance()); \
                return t->getOptionsDescription(); \
                } \
} RegisterTask##instance##Instance; 



#endif	/* TASK_H */

