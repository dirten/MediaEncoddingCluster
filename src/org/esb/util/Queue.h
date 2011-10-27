#ifndef ORG_ESB_UTIL_QUEUE
#define ORG_ESB_UTIL_QUEUE

#include <deque>
#include "QueueListener.h"
#include <boost/thread.hpp>
#include <boost/thread/condition.hpp>
#include "Log.h"
#include "org/esb/lang/Thread.h"
#include "exports.h"

#include "org/esb/io/ObjectOutputStream.h"
#include "org/esb/io/ObjectInputStream.h"
#include "org/esb/io/FileInputStream.h"
#include "org/esb/io/FileOutputStream.h"
#include "UUID.h"
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
        boost::mutex enqueue_mutex;
        boost::mutex dequeue_mutex;
        boost::condition queue_condition;
        boost::condition enqueue_condition;
        boost::condition dequeue_condition;
        boost::condition stop_condition;
        bool _is_waiting;
      public:
        bool _closed;
      public:

        Queue() {
          _listener = NULL;
          _closed = false;
          _is_waiting = false;
        }

        void flush() {
          LOGDEBUG("flushing queue");
          boost::mutex::scoped_lock enqueue_lock(queue_mutex);
          _closed = true;
          LOGDEBUG("clear Queue");
          _q.clear();
          LOGDEBUG("Queue cleared");
          dequeue_condition.notify_all();
          enqueue_condition.notify_all();
          LOGDEBUG("waiting Threads notified" << &queue_condition);
          _closed = false;
        }

        bool closed() {
          return _closed;
        }

        virtual ~Queue() {
          if (_is_waiting) {
            //LOGDEBUG("is wating")
            org::esb::lang::Thread::sleep2(5150);
            //boost::mutex::scoped_lock enqueue_lock(queue_mutex);
            //stop_condition.wait(enqueue_lock);
          }
          //LOGDEBUG("~Queue()")
          _q.clear();
          //queue_condition.notify_all();
        }

        bool enqueue(T obj) {
          //LOGTRACEMETHOD("enqueue(T obj)")
          boost::mutex::scoped_lock enqueue_lock(enqueue_mutex);
          if (_closed)return false;
          //LOGTRACE("after mutex");
          bool result = false;
          //bool first_item=_q.empty();
          while (MAXSIZE!=0 && _q.size() >= MAXSIZE) {
            //LOGDEBUG("Waiting in enqueuelock:"<<_q.size());
            enqueue_condition.wait(enqueue_lock);
            //LOGDEBUG("condition enqueuelock");
          }
          if (_closed)return false;

          {
            boost::mutex::scoped_lock queue_lock(queue_mutex);
            _q.push_back(obj);
          }
          result = true;
          //LOGTRACE("notify condition enqueuelock");
          //if(first_item)
          dequeue_condition.notify_one();
          return result;
        }

        T dequeue() {
          //LOGTRACEMETHOD("T dequeue()");
          boost::mutex::scoped_lock dequeue_lock(dequeue_mutex);
          //LOGTRACE("after mutex");
          while (_q.size() == 0) {
            dequeue_condition.wait(dequeue_lock);
            //LOGTRACE("condition dequeuelock");
            if (_closed) {
              //LOGTRACE("channel closed, returning");
              return T();
            }
          }
          T object;
          {
            boost::mutex::scoped_lock queue_lock(queue_mutex);
            object = _q.front();
            _q.pop_front();
          }
          //LOGTRACE("notify condition dequeuelock");
          enqueue_condition.notify_one();
          return object;
        }

        bool dequeueLock() {
          queue_mutex.lock();
        }

        bool dequeueUnLock() {
          queue_mutex.unlock();
        }

        T lookup() {
          T object;
          {
            boost::mutex::scoped_lock queue_lock(queue_mutex);
            object = _q.front();
          }
          return object;
        }

        bool dequeue(T & object) {
          //LOGTRACEMETHOD("dequeue(T & object)");
          boost::mutex::scoped_lock dequeue_lock(dequeue_mutex);
          if (_closed)return false;
          bool result = false;
          while (_q.size() == 0) {
            //LOGTRACE("Waiting in dequeuelock:"<<_q.size());
            dequeue_condition.wait(dequeue_lock);
            if (_closed) {
              //LOGTRACE("channel closed, returning");
              return false;
            }
            //LOGTRACE("condition dequeuelock");
          }
          if (_closed)return false;
          {
            boost::mutex::scoped_lock queue_lock(queue_mutex);
            object = _q.front();
            _q.pop_front();
          }
          result = true;
          enqueue_condition.notify_one();
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

        /*
        void setSize(int size){
          MAXSIZE=size;
        }
         */
        void setQueueListener(QueueListener * listener) {
          _listener = listener;
        }
      };

      template<typename T >
              class FileQueue {
        std::string _directory;
        Queue<std::string,0> _uuid_q;
        public:
        FileQueue(std::string dirname){
          _directory=dirname;
          org::esb::io::File dir(_directory.c_str());
          if (!dir.exists()) {
            dir.mkdir();
          }
        }
        
        std::string enqueue(T obj) {
          std::string name=_directory;
          name+="/";
          std::string uuid=PUUID();
          name+=uuid;

          org::esb::io::File out(name.c_str());
          org::esb::io::FileOutputStream fos(&out);
          org::esb::io::ObjectOutputStream ous(&fos);
          
          ous.writeObject(obj);
          ous.close();
          _uuid_q.enqueue(uuid);
          return uuid;
        }
        
        T dequeue() {
          T object;
          std::string name=_directory;
          name+="/";
          std::string uuid=_uuid_q.dequeue();
          name+=uuid;
          
          org::esb::io::File infile(name.c_str());
          org::esb::io::FileInputStream fis(&infile);
          org::esb::io::ObjectInputStream ois(&fis);
          ois.readObject(object);
          infile.deleteFile();
          return object;
        }
        int size() {
          return _uuid_q.size();
        }
      };

    }
  }
}
#endif
