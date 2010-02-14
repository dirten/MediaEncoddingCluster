#ifndef ORG_ESB_UTIL_QUEUE
#define ORG_ESB_UTIL_QUEUE

#include <deque>
#include "QueueListener.h"
#include <boost/thread.hpp>
#include <boost/thread/condition.hpp>
#include "Log.h"
namespace org {
  namespace esb {
    namespace util {

      template<typename T, int MAXSIZE = 10 >
          class Queue {
        classlogger("org.esb.util.Queue");
      private:
        std::deque<T> _q;
        QueueListener * _listener;
        boost::mutex queue_mutex;
        boost::condition queue_condition;
      public:

        Queue() {
          _listener = NULL;
        }

        void flush() {
          _q.clear();
          queue_condition.notify_all();
        }

        ~Queue() {
          //                    _q.clear();
          //                    queue_condition.notify_all();
        }

        bool enqueue(T obj) {
          LOGTRACEMETHOD("enqueue(T obj)")
          boost::mutex::scoped_lock enqueue_lock(queue_mutex);
          LOGTRACE("after mutex");
          bool result = false;
          while (_q.size() >= MAXSIZE) {
            LOGTRACE("Waiting in enqueuelock");
            queue_condition.wait(enqueue_lock);
            LOGTRACE("condition enqueuelock");
          }
          _q.push_back(obj);
          result = true;
          LOGTRACE("notify condition enqueuelock");
          queue_condition.notify_one();
          return result;
        }

        T dequeue() {
          LOGTRACEMETHOD("T dequeue()");
          boost::mutex::scoped_lock dequeue_lock(queue_mutex);
          LOGTRACE("after mutex");
          while (_q.size() == 0) {
            LOGTRACE("Waiting in dequeuelock");
            queue_condition.wait(dequeue_lock);
            LOGTRACE("condition dequeuelock");
          }
          T object = _q.front();
          _q.pop_front();
          LOGTRACE("notify condition dequeuelock");
          queue_condition.notify_one();
          return object;
        }

        bool dequeue(T object) {
          boost::mutex::scoped_lock dequeue_lock(queue_mutex);
          bool result = false;
          while (_q.size() == 0) {
            queue_condition.wait(dequeue_lock);
          }
          object = _q.front();
          _q.pop_front();
          result = true;
          queue_condition.notify_one();
          return result;
        }

        T operator[](int a) {
          return _q[a];
        }

        int size() {
          return _q.size();
        }

        void setQueueListener(QueueListener * listener) {
          _listener = listener;
        }
      };
    }
  }
}
#endif
