#ifndef ORG_ESB_HIVE_PROTOCOLLSERVER_H
#define ORG_ESB_HIVE_PROTOCOLLSERVER_H
#include <iostream>
#include "org/esb/lang/Runnable.h"
#include "org/esb/net/Socket.h"
#include "org/esb/util/StringUtil.h"
#include "ProtocolCommand.h"
#include "Command.h"
#include "CommandInputStream.h"

//#include "proto_command.h"
#include <list>

using namespace std;
using namespace org::esb::net;
using namespace org::esb::lang;
namespace org{
    namespace esb{
        namespace hive{
            class ProtocolServer:public Runnable{
                private:
            	    Socket * socket;
            	    list<ProtocolCommand*> l;
                    CommandInputStream * _cis;
		    pthread_mutex_t * mutex;
                public:
            	    ~ProtocolServer();
            	    ProtocolServer(Socket * socket);
            	    ProtocolServer(Socket * socket, pthread_mutex_t * m);
            	    void run();
            };
        }
    }
}
#endif


