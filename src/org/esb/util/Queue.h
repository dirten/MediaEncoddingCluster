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
      private:
        std::deque<T> _q;
        QueueListener * _listener;
        boost::mutex condition_mutex;
        boost::mutex enqueue_mutex;
        boost::mutex dequeue_mutex;
        boost::mutex queue_mutex;
        //        boost::mutex queue_thread_mutex;
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
          boost::mutex::scoped_lock enqueue_lock(enqueue_mutex);
          bool result=false;
          if (_q.size() >= MAXSIZE) {
            boost::mutex::scoped_lock condition_lock(condition_mutex);
            queue_condition.wait(condition_lock);
            //            queue_condition.wait(enqueue_lock);
          }
          {
            boost::mutex::scoped_lock enqueue_lock(queue_mutex);
            _q.push_back(obj);
            result=true;
          }
          queue_condition.notify_one();
          return result;
        }

        /*
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
         */
        T dequeue() {
          boost::mutex::scoped_lock enqueue_lock(dequeue_mutex);
          if (_q.size() == 0) {
            boost::mutex::scoped_lock condition_lock(condition_mutex);
            queue_condition.wait(condition_lock);
            //			  queue_condition.wait(dequeue_lock);
          }
          {
            boost::mutex::scoped_lock dequeue_lock(queue_mutex);
            T object = _q.front();
            //          object = _q.front();
            _q.pop_front();
            queue_condition.notify_one();
            return object;
          }
        }

        bool dequeue(T object) {
          boost::mutex::scoped_lock enqueue_lock(dequeue_mutex);
          bool result=false;
          if (_q.size() == 0) {
            boost::mutex::scoped_lock condition_lock(condition_mutex);
            queue_condition.wait(condition_lock);
            //			  queue_condition.wait(dequeue_lock);
          }
          {
            boost::mutex::scoped_lock dequeue_lock(queue_mutex);
            object = _q.front();
            _q.pop_front();
            result=true;
          }
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
