#ifndef ORG_ESB_UTIL_QUEUELISTENER
#define ORG_ESB_UTIL_QUEUELISTENER
namespace org {
  namespace esb {
    namespace util {
      enum QueueEvent {
        QEVENT_ENQUEUE,
        QEVENT_DEQUEUE,
        QEVENT_QFULL,
        QEVENT_QEMPTY
      };
      class QueueListener {
      public:
        virtual void onQueueEvent(QueueEvent) = 0;
      };
    }
  }
}
#endif
