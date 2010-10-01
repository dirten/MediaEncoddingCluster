#ifndef ORG_ESB_MQ_QUEUECONNECTION
#define ORG_ESB_MQ_QUEUECONNECTION
#include <string>
#include "MQConnection.h"
namespace org{
  namespace esb{
    namespace mq{
      class QueueConnection{
      public:
        QueueConnection(std::string url);
        QueueConnection(std::string host, int port);

        ~QueueConnection();
        void createQueue(std::string name);
        void deleteQueue(std::string name);
        bool queueExist(std::string name);
      private:
        safmq::MQConnection * _connection;
      };
    }
  }
}


#endif
