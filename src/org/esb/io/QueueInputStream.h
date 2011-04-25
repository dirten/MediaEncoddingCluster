/* 
 * File:   QueueInputStream.h
 * Author: HoelscJ
 *
 * Created on 16. September 2010, 14:50
 */

#ifndef QUEUEINPUTSTREAM_H
#define	QUEUEINPUTSTREAM_H
#include "InputStream.h"
#include "org/esb/lang/Ptr.h"
#include "exports.h"
namespace safmq {
  class MessageQueue;
}
namespace org {
  namespace esb {
    namespace io {

      class  QueueInputStream : public InputStream {
      public:
        QueueInputStream(std::string url);
        QueueInputStream(std::string host, int port, std::string queuename);
        QueueInputStream(const QueueInputStream& orig);
        virtual ~QueueInputStream();
        long long int available(bool isBlocking = false);
        int read(string & str);
        int read(unsigned char *buffer, int length);
        int read(vector < unsigned char >&buffer);
        int read();

      private:
        Ptr<safmq::MessageQueue> _queue;

      };
    }
  }
}

#endif	/* QUEUEINPUTSTREAM_H */

