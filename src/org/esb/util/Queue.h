#ifndef ORG_ESB_UTIL_QUEUE
#define ORG_ESB_UTIL_QUEUE

#include <queue>
#include "QueueListener.h"
#include <boost/thread.hpp>
#include "Log.h"
namespace org {
  namespace esb {
    namespace util {

      template <typename T, int MAXSIZE = 10 >
          class Queue {
      private:
        std::queue<T> _q;
        QueueListener * _listener;
        boost::mutex queue_mutex;
        boost::mutex enqueue_mutex;
        boost::mutex dequeue_mutex;
        boost::mutex queue_thread_mutex;
        boost::condition queue_condition;
      public:

        Queue() {
          _listener = NULL;
        }

        void enqueue(T obj) {
          boost::mutex::scoped_lock enqueue_lock(enqueue_mutex);
          if (_q.size() >= MAXSIZE) {
            if (_listener != NULL)
              _listener->onQueueEvent(QEVENT_QFULL);
//            boost::mutex::scoped_lock queue_lock(queue_mutex);
            queue_condition.wait(enqueue_lock);
          }
          _q.push(obj);
//          boost::mutex::scoped_lock queue_lock(queue_mutex);
          queue_condition.notify_all();
          if (_listener != NULL)
            _listener->onQueueEvent(QEVENT_ENQUEUE);
        }

        T dequeue(T obj) {
          boost::mutex::scoped_lock dequeue_lock(dequeue_mutex);
          if (_q.size() == 0) {
            if (_listener != NULL)
              _listener->onQueueEvent(QEVENT_QEMPTY);
//            boost::mutex::scoped_lock queue_lock(queue_mutex);
            queue_condition.wait(dequeue_lock);
          }
          T object;
          object = _q.front();
          _q.pop();
//          boost::mutex::scoped_lock queue_lock(queue_mutex);
          queue_condition.notify_all();
          if (_listener != NULL)
            _listener->onQueueEvent(QEVENT_DEQUEUE);
          return object;
        }

        T dequeue() {
          boost::mutex::scoped_lock dequeue_lock(dequeue_mutex);
          if (_q.size() == 0) {
            if (_listener != NULL)
              _listener->onQueueEvent(QEVENT_QEMPTY);
            boost::mutex::scoped_lock queue_lock(queue_mutex);
            queue_condition.wait(queue_lock);
          }
          T object= _q.front();
//          object = _q.front();
          _q.pop();
          boost::mutex::scoped_lock queue_lock(queue_mutex);
          queue_condition.notify_all();
          if (_listener != NULL)
            _listener->onQueueEvent(QEVENT_DEQUEUE);
          return object;
        }

        void setQueueListener(QueueListener * listener) {
          _listener = listener;
        }
      };
    }
  }
}
#endif
