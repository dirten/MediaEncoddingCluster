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
#include "boost/function.hpp"
#include "org/esb/util/Depricated.h"
#include "org/esb/av/Packet.h"
#include "org/esb/av/Codec.h"
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
          PREPARED,
          EXECUTE,
          CLEANUP,
          DONE,
          CANCEL,
          CANCELED,
          ERROR
        };
        enum PAD_TYPE {
          NOPAD,
          SOURCE,
          SINK
        };

      protected:
        void setProgressLength(unsigned int);
        unsigned int getProgressLength();
        void setProgress(unsigned int);
        void setStatus(STATUS);
        void setStatusMessage(std::string);
        //virtual void newBuffer(Ptr<org::esb::av::Packet>);

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
        DEPRECATED(virtual void setSource(std::string));
        DEPRECATED(virtual void setSink(std::string));
        virtual void addSourceTask(Ptr<Task>);
        virtual void addSinkTask(Ptr<Task>);
        virtual void pushBuffer(Ptr<org::esb::av::Packet>);
        virtual void pullBuffer(Ptr<org::esb::av::Packet>);
        //virtual void setBufferCodec(int, Ptr<org::esb::av::Codec>);
        virtual int getPadTypes();
        virtual void setUUID(std::string);
        virtual std::string getUUID();
        virtual void addStatusObserver( boost::function<void (Task*)> func);
        virtual void addProgressObserver( boost::function<void (Task*)> func);
        virtual void cancel();
        bool    isCanceled();
      private:
        unsigned int _progress;
        unsigned int _progress_length;
        STATUS _status;
        std::string _status_message;
        std::string _source;
        std::string _sink;
        std::string _uuid;
        boost::function<void (Task*)> statusObserver;
        boost::function<void (Task*)> progressObserver;
        std::list<Ptr<Task> > _sources;
        std::list<Ptr<Task> > _sinks;
        //std::map<int, Ptr<org::esb::av::Codec> > _buffer_codec;
        //bool _isCanceled;
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

