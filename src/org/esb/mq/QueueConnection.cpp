#include "org/esb/mq/QueueConnection.h"
#include "MQFactory.h"
#include "safmq.h"
//#include "org/esb/util/Log.h"
#include "org/esb/util/StringUtil.h"
#include <exception>
#include "QueueException.h"

#include <boost/bind.hpp>
#include "org/esb/util/StringTokenizer.h"
#include "org/esb/lang/Thread.h"
namespace org{
  namespace esb{
    namespace mq{
      class Selector{
      public:
        Selector(std::string sel){
          org::esb::util::StringTokenizer tok(sel, "=");
          if(tok.countTokens()==2){
            _name=tok.nextToken();
            _value=tok.nextToken();
          }
        }
        Selector(){}
        bool match(QueueMessage & msg){
          bool result=false;
          //LOGDEBUG("Label="<<msg.getLabel()<<" Value="<<_value);
          if(_value.length()==0||msg.getLabel()==_value)
            result=true;
          return result;
        }
      private:
        std::string _name;
        std::string _value;
      };

      QueueConnection::QueueConnection(std::string url){
        _selector=NULL;
        try {
          /*url="safmq://admin:@localhost:9000/testqueue"*/
          _connection = safmq::MQFactory::BuildConnection(url, "", "");
        } catch (tcpsocket::SocketException & ex) {
          //LOGERROR(ex.what());
          //LOGERROR(ex.getError());
          throw QueueException("could not connect to QueueManager");
        }
      }
      QueueConnection::QueueConnection(std::string host, int port){
        _selector=NULL;
        try {
          /*url="safmq://admin:@localhost:9000/testqueue"*/
          std::string url="safmq://admin:@";
          url+=host;
          url+=":";
          url+=org::esb::util::StringUtil::toString(port);
          _connection = safmq::MQFactory::BuildConnection(url, "", "");
        } catch (tcpsocket::SocketException & ex) {
          //LOGERROR(ex.what());
          //LOGERROR(ex.getError());
          throw QueueException("could not connect to QueueManager");
        }
      }
      QueueConnection::~QueueConnection(){
        delete _connection;
        delete _selector;
      }
      void QueueConnection::createQueue(std::string name){
        safmq::ErrorCode ec=_connection->CreateQueue(name);
        if(safmq::EC_NOERROR!=ec)
          throw QueueException("could not create queue "+name);
      }
      void QueueConnection::deleteQueue(std::string name){
        safmq::ErrorCode ec=_connection->DeleteQueue(name);
        if(safmq::EC_NOERROR!=ec)
          throw QueueException("could not delete queue "+name);
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
      
      void QueueConnection::setMessageListener(std::string q_name,QueueMessageListener & listener, std::string selector){
        _listener=&listener;
        _listener_queue=q_name;
        _listener_selector=selector;
        if(selector.length()>0)
          _selector=new Selector(selector);
      }

      void QueueConnection::startListener(){
        _listener_thread=boost::thread(boost::bind(&QueueConnection::startListenerThread, this));
      }
      void QueueConnection::stopListener(){
        _listener_thread.interrupt();
        _listener_thread.join();
      }
      
      void QueueConnection::startListenerThread(){
        Ptr<safmq::MessageQueue> queue=getMessageQueue(_listener_queue);
        safmq::MQConnection::CursorHandle cursor;
        queue->OpenCursor(cursor);
        while(true){
          boost::this_thread::interruption_point();
          //LOGDEBUG("roundup");
          QueueMessage msg;
          queue->PeekCursor(false,cursor,msg);
          bool match=true;
          safmq::ErrorCode err;
          if(_selector)
            match=_selector->match(msg);
          if(match){
            err=queue->RetrieveCursor(true,cursor, msg);
            if(err==safmq::EC_NOERROR)
              _listener->onMessage(msg);
          }
          err=queue->AdvanceCursor(cursor);
          if(!err==safmq::EC_NOERROR){
            org::esb::lang::Thread::sleep2(1000);
            //LOGDEBUG("no more message, waiting!!!")
            queue->CloseCursor(cursor);
            queue->OpenCursor(cursor);
          }
        }
      }

      Ptr<safmq::MessageQueue> QueueConnection::getMessageQueue(std::string q_name){
        try{
          return new safmq::MessageQueue(_connection, q_name);
        }catch(safmq::ErrorCode & error){
          std::cout << "Error get queue : "<<error<<std::endl;
          //LOGERROR(error);
          //LOGERROR(error.getError());
          throw QueueException("could not get Queue "+q_name);
        }
      }
      
      void QueueConnection::enqueue(std::string q_name,QueueMessage & msg){
        safmq::MQConnection::QueueHandle qhandle;
        _connection->OpenQueue(q_name, qhandle);
        _connection->Enqueue(qhandle, msg);
      }

      
      void QueueConnection::dequeue(std::string q_name, QueueMessage & msg){
        safmq::MQConnection::QueueHandle qhandle;
        _connection->OpenQueue(q_name, qhandle);
        _connection->Retrieve(qhandle,true,-1, msg);
      }
    }
  }
}

