/* 
 * File:   QManager.h
 * Author: HoelscJ
 *
 * Created on 17. September 2010, 11:49
 */

#ifndef QMANAGER_H
#define	QMANAGER_H
#include <string>
//#include "Log.h"

namespace org {
  namespace esb {
    namespace mq {

      class QueueManager {
      public:
        QueueManager(std::string base_dir);
        virtual ~QueueManager();
        void createQueue(std::string name);
        void start();
        void stop();
        std::string getUrl();
      private:
        QueueManager(const QueueManager& orig);
        int numForwardThreads;
        bool _running;
      };
    }
  }
}

#endif	/* QMANAGER_H */

