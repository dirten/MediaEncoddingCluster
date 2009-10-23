#ifndef ORG_ESB_HIVE_HIVELISTENER_H
#define ORG_ESB_HIVE_HIVELISTENER_H

#include "org/esb/signal/MessageListener.h"
#include "org/esb/signal/Message.h"
#include "org/esb/net/TcpServerSocket.h"
#include "org/esb/lang/Thread.h"
//#include <boost/thread.hpp>
#include <list>

namespace org {
    namespace esb {
        namespace hive {

            class HiveListener : public org::esb::signal::MessageListener {
            public:
                HiveListener();
                ~HiveListener();
                void onMessage(org::esb::signal::Message & msg);

                void startListener();
            private:
                bool main_nextloop;
                bool is_running;
                //		boost::thread *t;
                org::esb::net::TcpServerSocket * server;
                list<org::esb::lang::Thread*> _clients;
            };
        }
    }
}
#endif

