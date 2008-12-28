#include "Queue.h"


namespace org{
	namespace esb{
		namespace util{
			class test{
			};
			class QTest:public QueueListener{
			public:
				QTest(){
					Queue<test*> q;
					q.setQueueListener(this);
					q.enqueue(new test());
				}
				void onQueueEvent(QueueEvent event){
				}
			};
		}
	}
}