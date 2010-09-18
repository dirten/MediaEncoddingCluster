/* 
 * File:   QManager.h
 * Author: HoelscJ
 *
 * Created on 17. September 2010, 11:49
 */

#ifndef QMANAGER_H
#define	QMANAGER_H
#include <inttypes.h>
#include "param_config.h"
#include "QManager.h"
#include "ServerThread.h"
#include "SystemDelivery.h"
#include "ForwardThread.h"
#include "Log.h"


namespace org {
  namespace esb {
    namespace mq {

      class QueueManager {
      public:
        QueueManager();
        QueueManager(const QueueManager& orig);
        virtual ~QueueManager();
        void createQueue(std::string name);
        void start();
        void stop();
      private:

        /// The global instance of the network serevr thread
        ServerThread server;
        /// The global instance of the TTL Error Delivery thread
        SystemDelivery sysdeliver;
        /// The global instance of the message forwarding thread
        ForwardThread* forwardthds;
        int numForwardThreads;
        safmq::Log* plog;
        bool _running;
      };
    }
  }
}

#endif	/* QMANAGER_H */

