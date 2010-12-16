/* 
 * File:   QManager.h
 * Author: HoelscJ
 *
 * Created on 17. September 2010, 11:49
 */

#ifndef QMANAGER_H
#define	QMANAGER_H
#include <string>
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
        std::string getUrl();
      private:

        int numForwardThreads;
        bool _running;
      };
    }
  }
}

#endif	/* QMANAGER_H */

