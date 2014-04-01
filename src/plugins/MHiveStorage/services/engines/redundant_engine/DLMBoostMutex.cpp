#include "DLMBoostMutex.h"
#include "org/esb/util/Foreach.h"
#include "org/esb/util/Log.h"

#include "boost/bind.hpp"

namespace mhivestorage{
  namespace engines {

    DLMBoostMutex::DLMBoostMutex(int self_port, std::vector<std::string>hosts)
    {
      _selfHost=new DLMHost(self_port);
      _isWaiting=false;
      foreach(std::string h, hosts){
        DLMHost*host = new DLMHost(h);
        _hosts.push_back(host);

        host->receiveLockRequest=boost::bind(&DLMBoostMutex::reveiveLockRequest, this);
        host->receiveLockRequestAnswer=boost::bind(&DLMBoostMutex::reveiveLockRequestAnswer, this, _1);
        host->receiveUnlockRequest=boost::bind(&DLMBoostMutex::reveiveUnlockRequest, this);
      }
    }

    void DLMBoostMutex::lock(){
      boost::mutex::scoped_lock lock(*this);
      sendLockRequest();
      _isWaiting=true;
      _condition.wait(lock);
    }

    void DLMBoostMutex::unlock(){
      _condition.notify_one();
      _isWaiting=false;
    }

    void DLMBoostMutex::sendLockRequest(){
      foreach(DLMHost * host, _hosts){
        LOGDEBUG("sending lock request to host:"<<host);
        host->sendLockRequest();
      }
    }

    void DLMBoostMutex::reveiveLockRequest(){
      LOGDEBUG("reveiver reveiveLockRequest");
    }

    void DLMBoostMutex::reveiveLockRequestAnswer(DLMHost * host){
      LOGDEBUG("reveiver reveiveLockRequestAnswer:"<<host);
    }

    void DLMBoostMutex::sendUnlockRequest(){
      foreach(DLMHost * host, _hosts){
        host->sendUnlockRequest();
      }
    }

    void DLMBoostMutex::reveiveUnlockRequest(){
      LOGDEBUG("reveiver reveiveUnlockRequest");
    }
  }
}
