#ifndef ORG_ESB_HIVE_PROTOCOLLSERVER_H
#define ORG_ESB_HIVE_PROTOCOLLSERVER_H
#include <iostream>
#include "org/esb/lang/Runnable.h"
#include "org/esb/net/TcpSocket.h"
#include "org/esb/util/StringUtil.h"
#include "ProtocolCommand.h"
//#include "Command.h"
//#include "CommandInputStream.h"
#include "org/esb/util/Log.h"
//#include "proto_command.h"
#include <list>

using namespace std;
using namespace org::esb::net;
using namespace org::esb::lang;
namespace org {
  namespace esb {
    namespace plugin {

      classlogger("org.esb.hive.ProtocolServer")
      class ProtocolServer : public Runnable {
      private:
        TcpSocket * socket;
        list<ProtocolCommand*> l;
        //CommandInputStream * _cis;
        //		    pthread_mutex_t * mutex;
      public:
        ~ProtocolServer();
        ProtocolServer(TcpSocket * socket);
        void run();
        void close();
      };
    }
  }
}
#endif


