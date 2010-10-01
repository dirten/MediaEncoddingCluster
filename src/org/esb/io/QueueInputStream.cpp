/* 
 * File:   QueueInputStream.cpp
 * Author: HoelscJ
 * 
 * Created on 16. September 2010, 14:50
 */

#include "QueueInputStream.h"
#include "MQFactory.h"
#include "MessageQueue.h"
#include "org/esb/util/Log.h"
#include "org/esb/util/StringUtil.h"
namespace org {
  namespace esb {
    namespace io {

      QueueInputStream::QueueInputStream(std::string url) {
        try {
          /*url="safmq://admin:@localhost:9000/testqueue"*/
          _queue = safmq::MQFactory::BuildQueueConnection(url, "", "");
        } catch (tcpsocket::SocketException & ex) {
          LOGERROR(ex.what());
          LOGERROR(ex.getError());
        }

      }
      QueueInputStream::QueueInputStream(std::string host, int port, std::string queuename) {
        try {
          std::string url="safmq://admin:@";
          url+=host;
          url+=":";
          url+=org::esb::util::StringUtil::toString(port);
          url+="/";
          url+=queuename;

          /*url="safmq://admin:@localhost:9000/testqueue"*/
          _queue = safmq::MQFactory::BuildQueueConnection(url, "", "");
        } catch (tcpsocket::SocketException & ex) {
          LOGERROR(ex.what());
          LOGERROR(ex.getError());
        }

      }

      QueueInputStream::QueueInputStream(const QueueInputStream& orig) {
      }

      QueueInputStream::~QueueInputStream() {
      }

      long long int QueueInputStream::available(bool isBlocking) {
        return 0;
      }

      int QueueInputStream::read(string & str) {
        safmq::QueueMessage msg;
        _queue->Retrieve(true,-1,msg);
        size_t length=msg.getBufferSize();
        std::istream*buf=msg.getBufferStream();
        char * data=new char[length];
        buf->read(data, length);
        
        str=std::string(data,length);
        delete []data;
        return 1;
      }

      int QueueInputStream::read(unsigned char *buffer, int length) {
        return 0;
      }

      int QueueInputStream::read(vector < unsigned char >&buffer) {
        return 0;
      }

      int QueueInputStream::read() {
        return 0;
      }

    }
  }
}
