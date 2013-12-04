#ifndef ORG_ESB_SIGNAL_MESSENGER_H
#define ORG_ESB_SIGNAL_MESSENGER_H

//#define BOOST_USE_WINDOWS_H
#include <string>
#include <map>
#include <list>
#include "MessageListener.h"
#include "boost/thread/mutex.hpp"
#include "org/esb/util/Log.h"
#define DEFAULT_NAME "global"
namespace org {
  namespace esb {
    namespace hive {
      const static std::string RESTART = "restart";
      const static std::string STOP = "stop";
      const static std::string START = "start";

    }
    namespace signal {
      class SIGNAL_EXPORT Messenger {
          classlogger("org.esb.signal.Messenger")
          public:
          static void free();
          void freeListener(std::string name = DEFAULT_NAME);
          static Messenger & getInstance(std::string name = DEFAULT_NAME);
          ~Messenger();
          void addMessageListener(MessageListener &listener, std::string name = DEFAULT_NAME);
          void removeMessageListener(MessageListener &listener, std::string name = DEFAULT_NAME);
          void sendMessage(Message & msg, std::string name = DEFAULT_NAME);
          void sendRequest(Message & msg, std::string name = DEFAULT_NAME);
        private:
          static boost::mutex request_mutex;
          static boost::mutex messenger_mutex;
          static std::map<std::string, Messenger*> messenger;
          Messenger(std::string name);
          std::map<std::string, std::list<MessageListener*> > listener;
      };
    }
  }
}
#endif
