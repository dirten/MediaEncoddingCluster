#ifndef ORG_ESB_UTIL_QUEUE
#define ORG_ESB_UTIL_QUEUE

#include <queue>
#include "QueueListener.h"
namespace org{
	namespace esb{
		namespace util{
			template <typename T>
			class Queue {
			private:
				std::queue<T> _q;
			public:
				void enqueue(T & obj){
					_q.push(obj);
				}
				void dequeue(T & obj){
					_q.pop(obj);
				}
				void setQueueListener(QueueListener & listener){

				}
			};
		}
	}
}
#endif
