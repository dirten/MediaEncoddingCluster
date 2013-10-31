#ifndef ORG_ESB_HIVE_HIVELISTENER_H
#define ORG_ESB_HIVE_HIVELISTENER_H

#include "org/esb/net/TcpServerSocket.h"
#include "org/esb/signal/MessageListener.h"
#include "org/esb/signal/Message.h"
#include "org/esb/lang/Thread.h"
#include "org/esb/util/Log.h"
#include "org/esb/core/ServicePlugin.h"

//#include <boost/thread.hpp>
#include <list>
#include "exports.h"
namespace org {
  namespace esb {
    namespace plugin {

      class HIVESERVICE_EXPORT HiveListener : public org::esb::core::ServicePlugin{
        classlogger("org.esb.hive.HiveListener")
      public:
        HiveListener();
        ~HiveListener();
        void onMessage(org::esb::signal::Message & msg);
        void startService();
        void stopService();

        void startListener();
        org::esb::core::OptionsDescription getOptionsDescription();
        org::esb::core::ServicePlugin::ServiceType getServiceType();
      private:
        bool main_nextloop;
        bool is_running;
        //		boost::thread *t;
        org::esb::net::TcpServerSocket * server;
        list<org::esb::lang::Thread*> _clients;
      };
      REGISTER_SERVICE("hivelistener", HiveListener)
    }
  }
}
#endif

