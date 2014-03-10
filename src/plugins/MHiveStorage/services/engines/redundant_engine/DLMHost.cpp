#include "DLMHost.h"
#include "org/esb/util/Log.h"
#include "org/esb/util/StringTokenizer.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/Net/NetException.h"
#include "org/esb/lang/Thread.h"
using org::esb::util::StringTokenizer;
namespace mhivestorage{
  namespace engines {

    DLMHost::DLMHost(std::string h)
    {
      StringTokenizer tok(h,":");
      port=0;

      if(tok.countTokens()==2){
        hostname=tok.nextToken();
        port=atoi(tok.nextToken().c_str());
      }
      LOGDEBUG("host:"<<hostname<<" port="<<port);
      Poco::Net::SocketAddress sa(hostname, port);
      dgs=Poco::Net::DatagramSocket(sa, true);
      //dgs.setReuseAddress(true);

      //go(DLMHost::receiveLoop, this);
    }

    DLMHost::DLMHost(int p)
    {
      this->port=p;
      Poco::Net::SocketAddress sa(Poco::Net::IPAddress(), port);
      //Poco::Net::SocketAddress sa("localhost", port);
      dgs=Poco::Net::DatagramSocket(sa, true);
      LOGDEBUG("host listen to Address:"<<dgs.address().toString())
      dgs.connect(sa);
      go(DLMHost::receiveLoop, this);
    }

    void DLMHost::sendLockRequest()
    {
      LOGDEBUG("send to Address:"<<dgs.address().toString())
      std::string msg=hostname;
      try{
        dgs.sendTo(msg.data(), msg.size(),dgs.address());
      }catch (Poco::Net::NetException & ex){
        LOGERROR("excpetion:"<<ex.displayText())
      }

      //receiveLockRequestAnswer(this);
    }


    void DLMHost::sendUnlockRequest()
    {

    }

    void DLMHost::receiveLoop()
    {
      char buffer[1024];
      while(true){
        Poco::Net::SocketAddress sender;
        int read=dgs.receiveFrom(buffer,sizeof(buffer)-1,sender);
        buffer[read]='\n';
        LOGDEBUG(dgs.address().toString()<<" Buffer loop received:"<<buffer<<" from "<<sender.toString());
      }
      //LOGDEBUG("leave loop")
    }

    /*
    void DLMHost::reveiveLockRequestAnswer()
    {

    }

    void DLMHost::reveiveLockRequest()
    {

    }
    void DLMHost::reveiveUnlockRequest()
    {

    }*/

  }
}
