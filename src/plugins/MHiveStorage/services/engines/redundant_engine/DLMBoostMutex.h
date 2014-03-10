#ifndef DLMBOOSTMUTEX_H
#define DLMBOOSTMUTEX_H
#include <vector>

#include "boost/thread/mutex.hpp"
#include "boost/thread/condition.hpp"

#include "DLMHost.h"
namespace mhivestorage{
  namespace engines {

    class DLMBoostMutex : public boost::mutex
    {
      public:
        DLMBoostMutex(int self_port, std::vector<std::string>hosts);
        void lock();
        void unlock();
      private:
        void sendLockRequest();
        void reveiveLockRequestAnswer(DLMHost * host);
        void reveiveLockRequest();

        void sendUnlockRequest();
        void reveiveUnlockRequest();

        boost::condition _condition;
        std::string _name;
        bool _isWaiting;
        std::vector<DLMHost*> _hosts;
        DLMHost * _selfHost;
    };
  }
}
#endif // DLMBOOSTMUTEX_H
