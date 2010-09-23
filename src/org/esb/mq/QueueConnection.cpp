#include "org/esb/mq/QueueConnection.h"
#include "MQFactory.h"
#include "safmq.h"
#include "org/esb/util/Log.h"
#include <exception>
namespace org{
  namespace esb{
    namespace mq{

      QueueConnection::QueueConnection(std::string url){
        try {
          /*url="safmq://admin:@localhost:9000/testqueue"*/
          _connection = safmq::MQFactory::BuildConnection(url, "", "");
        } catch (tcpsocket::SocketException & ex) {
          //LOGERROR(ex.what());
          //LOGERROR(ex.getError());
          throw ex;
        }
      }
      QueueConnection::~QueueConnection(){
        delete _connection;
      }
      void QueueConnection::createQueue(std::string name){
        safmq::ErrorCode ec=_connection->CreateQueue(name);
        if(safmq::EC_NOERROR!=ec)
          throw std::exception();
      }
      void QueueConnection::deleteQueue(std::string name){
        safmq::ErrorCode ec=_connection->DeleteQueue(name);
        if(safmq::EC_NOERROR!=ec)
          throw std::exception();
      }
      bool QueueConnection::queueExist(std::string name){
        bool result=false;
        safmq::MQConnection::QDATA_VECTOR vec;
        _connection->EnumerateQueues(vec);
        safmq::MQConnection::QDATA_VECTOR::iterator it=vec.begin();
        for(;it!=vec.end();it++){
          if((*it).queuename==name){
            result=true;
            break;
          }
        }
        return result;
      }
    }
  }
}

