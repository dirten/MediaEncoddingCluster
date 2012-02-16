#ifndef ORG_ESB_HIVE_PROTOCOLLSERVER_H
#define ORG_ESB_HIVE_PROTOCOLLSERVER_H
#include <iostream>
//#include "org/esb/lang/Runnable.h"
//#include "org/esb/net/TcpSocket.h"
//#include "org/esb/util/StringUtil.h"
#include "ProtocolCommand.h"
//#include "Command.h"
//#include "CommandInputStream.h"
#include "org/esb/util/Log.h"
//#include "proto_command.h"
#include <list>

using namespace std;
using namespace org::esb;
using namespace org::esb::lang;
namespace org {
  namespace esb {
    namespace net{
      class TcpSocket;
    }
    namespace plugin {

      classlogger("org.esb.plugin.ProtocolServer");
      class ProtocolServer {
      private:
        net::TcpSocket * socket;
        list<ProtocolCommand*> l;
      public:
        ~ProtocolServer();
        ProtocolServer(net::TcpSocket * socket);
        void run();
        void close();
      };
    }
  }
}
#endif


