/* 
 * File:   QueueOutputStream.h
 * Author: HoelscJ
 *
 * Created on 16. September 2010, 13:50
 */

#ifndef QUEUEOUTPUTSTREAM_H
#define	QUEUEOUTPUTSTREAM_H
#include "OutputStream.h"
#include "org/esb/lang/Ptr.h"
#include "org/esb/io/exports.h"
#pragma warning( disable: 4251 )
namespace safmq{
  class MessageQueue;
}
namespace org {
  namespace esb {
    namespace io {

      class IO_EXPORT QueueOutputStream : public OutputStream {
      public:
        QueueOutputStream(std::string url);
        QueueOutputStream(std::string host, int port, std::string queuename);
        QueueOutputStream(const QueueOutputStream& orig);
        virtual ~QueueOutputStream();
        void write(char buffer);
        void write(char * buffer, int length);
        void write(vector <unsigned char >& buffer);
        bool close();
        void flush();

      private:
        Ptr<safmq::MessageQueue> _queue;
      };
    }
  }
}

#endif	/* QUEUEOUTPUTSTREAM_H */

