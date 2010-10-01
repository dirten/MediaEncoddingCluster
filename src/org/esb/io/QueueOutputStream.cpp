/* 
 * File:   QueueOutputStream.cpp
 * Author: HoelscJ
 * 
 * Created on 16. September 2010, 13:50
 */

#include "QueueOutputStream.h"
#include "MQFactory.h"
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
        safmq::QueueMessage msg;
        msg.setLabel("nolabel");
        msg.getBufferStream()->write(buffer, length);
        _queue->Enqueue(msg);

      }

      void QueueOutputStream::write(vector <unsigned char >& buffer) {
      }

      void QueueOutputStream::close() {
      }

      void QueueOutputStream::flush() {
      }
    }
  }
}
