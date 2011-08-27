#ifndef ORG_ESB_MQ_QUEUECONNECTION
#define ORG_ESB_MQ_QUEUECONNECTION
#include <string>
#include "MQConnection.h"
#include "MessageQueue.h"
#include "QueueMessageListener.h"
#include <boost/thread.hpp>

namespace org{
  namespace esb{
    namespace mq{
      class Selector;
      class QueueConnection{
      public:
        QueueConnection(std::string url);
        QueueConnection(std::string host, int port);

        ~QueueConnection();
        void createQueue(std::string name);
        void deleteQueue(std::string name);
        bool queueExist(std::string name);
        void setMessageListener(std::string q_name,QueueMessageListener & listener, std::string selector=std::string());
        void enqueue(std::string q_name,QueueMessage &msg);
        void dequeue(std::string q_name,QueueMessage &msg);
        void startListener();
        void stopListener();
        safmq::MessageQueue * getMessageQueue(std::string q_name);
      private:
        safmq::MQConnection * _connection;
        QueueMessageListener * _listener;
        std::string _listener_queue;
        std::string _listener_selector;
        void startListenerThread();
        boost::thread _listener_thread;
        Selector * _selector;
      };
    }
  }
}


#endif
