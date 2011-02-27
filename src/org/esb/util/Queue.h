#ifndef ORG_ESB_UTIL_QUEUE
#define ORG_ESB_UTIL_QUEUE

#include <deque>
#include "QueueListener.h"
#include <boost/thread.hpp>
#include <boost/thread/condition.hpp>
#include "Log.h"
#include "org/esb/lang/Thread.h"
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
        boost::condition stop_condition;
        bool _is_waiting;
        bool _closed;
      public:

        Queue() {
          _listener = NULL;
          _closed=false;
          _is_waiting=false;
        }

        void flush() {
          LOGDEBUG("flushing queue");
         boost::mutex::scoped_lock enqueue_lock(queue_mutex);
          LOGDEBUG("clear Queue");
          _q.clear();
          LOGDEBUG("Queue cleared");
          _closed=true;
          queue_condition.notify_all();
          LOGDEBUG("waiting Threads notified"<<&queue_condition);
        }
        
        virtual ~Queue() {
          if(_is_waiting){
            LOGDEBUG("is wating")
            org::esb::lang::Thread::sleep2(150);
            //boost::mutex::scoped_lock enqueue_lock(queue_mutex);
            //stop_condition.wait(enqueue_lock);
          }
          std::cout << " ~Queue()"<<std::endl;
          _q.clear();
          //queue_condition.notify_all();
        }

        bool enqueue(T obj) {
          //LOGTRACEMETHOD("enqueue(T obj)")
          if(_closed)return false;
          boost::mutex::scoped_lock enqueue_lock(queue_mutex);
          LOGTRACE("after mutex");
          bool result = false;
          while (_q.size() >= MAXSIZE) {
            _is_waiting=true;
            LOGDEBUG("Waiting in enqueuelock");
            queue_condition.wait(enqueue_lock);
            _is_waiting=false;
            stop_condition.notify_one();
            LOGDEBUG("condition enqueuelock");
          }
          _q.push_back(obj);
          result = true;
          LOGTRACE("notify condition enqueuelock");
          queue_condition.notify_one();
          return result;
        }

        T dequeue() {
          //LOGTRACEMETHOD("T dequeue()");
          boost::mutex::scoped_lock dequeue_lock(queue_mutex);
          //LOGTRACE("after mutex");
          while (_q.size() == 0) {
            LOGTRACE("Waiting in dequeuelock"<<&queue_condition);
            queue_condition.wait(dequeue_lock);
            LOGTRACE("condition dequeuelock");
          }
          T object = _q.front();
          _q.pop_front();
          //LOGTRACE("notify condition dequeuelock");
          queue_condition.notify_one();
          return object;
        }

        bool dequeue(T & object) {
          LOGTRACEMETHOD("dequeue(T & object)");
          if(_closed)return false;
          boost::mutex::scoped_lock dequeue_lock(queue_mutex);
          bool result = false;
          while (_q.size() == 0) {
            LOGTRACE("Waiting in dequeuelock"<<&queue_condition);
            _is_waiting=true;
            queue_condition.wait(dequeue_lock);
            stop_condition.notify_one();
            _is_waiting=false;
            if(_closed){
              LOGTRACE("channel closed, returning");
              return false;
            }
            LOGTRACE("condition dequeuelock");
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
        /*
        T operator<<(  T data)
        {
          enqueue(data);
            return data;
        }

        T operator>>(  T & data)
        {
          data=dequeue();
          return data;
        }
        */
        int size() {
		  boost::mutex::scoped_lock dequeue_lock(queue_mutex);
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
