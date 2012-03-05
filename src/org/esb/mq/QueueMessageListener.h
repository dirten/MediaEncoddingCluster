#ifndef QUEUEMESSAGELISTENER_H
#define QUEUEMESSAGELISTENER_H
#include "QueueMessage.h"
#include "ObjectMessage.h"
namespace org{
    namespace esb{
        namespace mq{

            class QueueMessageListener
            {
            public:
              virtual void onMessage(QueueMessage & msg)=0;
            };
        }
    }
}

#endif // QUEUEMESSAGELISTENER_H
