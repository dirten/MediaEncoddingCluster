/* 
 * File:   QueueOutputStream.cpp
 * Author: HoelscJ
 * 
 * Created on 16. September 2010, 13:50
 */

#include "QueueOutputStream.h"
#include "lib/MQFactory.h"
#include "MessageQueue.h"
#include "org/esb/util/Log.h"
#include "org/esb/util/StringUtil.h"
namespace org {
  namespace esb {
    namespace io {

      QueueOutputStream::QueueOutputStream(std::string url) {
        try {
          /*url="safmq://admin:@localhost:9000/testqueue"*/
          _queue = safmq::MQFactory::BuildQueueConnection(url, "", "");
        } catch (tcpsocket::SocketException & ex) {
          LOGERROR(ex.what());
          LOGERROR(ex.getError());
        }
      }

      QueueOutputStream::QueueOutputStream(std::string host, int port, std::string queuename) {
        try {
          std::string url = "safmq://admin:@";
          url += host;
          url += ":";
          url += org::esb::util::StringUtil::toString(port);
          url += "/";
          url += queuename;

          /*url="safmq://admin:@localhost:9000/testqueue"*/
          _queue = safmq::MQFactory::BuildQueueConnection(url, "", "");
        } catch (tcpsocket::SocketException & ex) {
          LOGERROR(ex.what());
          LOGERROR(ex.getError());
        }
      }

      QueueOutputStream::QueueOutputStream(const QueueOutputStream& orig) {

      }

      QueueOutputStream::~QueueOutputStream() {
      }

      void QueueOutputStream::write(char buffer) {
      }

      void QueueOutputStream::write(char * buffer, int length) {
        if(!_queue)throw std::exception();
        safmq::QueueMessage msg;
        msg.setLabel("nolabel");
        msg.getBufferStream()->write(buffer, length);
        safmq::ErrorCode er;
        if((er=_queue->Enqueue(msg))!=safmq::EC_NOERROR){
          LOGERROR(er);
          throw std::exception();
        }
      }

      void QueueOutputStream::write(vector <unsigned char >& buffer) {
      }

      bool QueueOutputStream::close() {
        return true;
      }

      void QueueOutputStream::flush() {
      }
    }
  }
}
