#ifndef ORG_ESB_UTIL_QUEUE
#define ORG_ESB_UTIL_QUEUE

#include <queue>
#include "QueueListener.h"
namespace org{
	namespace esb{
		namespace util{
			template <typename T, int MAXSIZE=10>
			class Queue {
			private:
				std::queue<T> _q;
				QueueListener * _listener;
			public:
				
				void enqueue(T obj){
					_q.push(obj);
					_listener->onQueueEvent(QEVENT_ENQUEUE);
					if(_q.size()>=MAXSIZE)
						_listener->onQueueEvent(QEVENT_QFULL);
				}

				void dequeue(T obj){
					_q.pop(obj);
					_listener->onQueueEvent(QEVENT_DEQUEUE);
					if(_q.size()==0)
						_listener->onQueueEvent(QEVENT_QEMPTY);
				}

				void setQueueListener(QueueListener * listener){
					_listener=listener;
				}
			};
		}
	}
}
#endif
