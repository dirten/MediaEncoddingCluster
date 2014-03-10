#ifndef DLMHOST_H
#define DLMHOST_H
#include <string>
#include "boost/function.hpp"
#include "Poco/Net/DatagramSocket.h"

namespace mhivestorage{
  namespace engines {
    class DLMHost
    {
      public:
        DLMHost(std::string host);
        DLMHost(int port);
        void sendLockRequest();
        //void reveiveLockRequestAnswer();
        //void reveiveLockRequest();

        void sendUnlockRequest();
        //void reveiveUnlockRequest();

        boost::function<void()> receiveLockRequest;
        boost::function<void(DLMHost*host)> receiveLockRequestAnswer;
        boost::function<void()> receiveUnlockRequest;
      private:
        std::string hostname;
        int port;
        Poco::Net::DatagramSocket dgs;
        void receiveLoop();

    };
  }
}
#endif // DLMHOST_H
