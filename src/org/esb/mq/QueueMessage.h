/* 
 * File:   QueueMessage.h
 * Author: HoelscJ
 *
 * Created on 17. Dezember 2010, 11:55
 */

#ifndef QUEUEMESSAGE_H
#define	QUEUEMESSAGE_H
#include <inttypes.h>
#include "safmq.h"
namespace org {
  namespace esb {
    namespace mq {

      class QueueMessage:public safmq::QueueMessage{
      public:
        QueueMessage();
        virtual ~QueueMessage();
      private:

      };
    }
  }
}

#endif	/* QUEUEMESSAGE_H */

