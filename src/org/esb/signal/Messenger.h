#ifndef ORG_ESB_SIGNAL_MESSENGER_H
#define ORG_ESB_SIGNAL_MESSENGER_H
#include <string>
#include <map>
#include <list>
#include "MessageListener.h"
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
            //class MessageListener;

            class Messenger {
              classlogger("org.esb.signal.Messenger")
            public:
                //    static const char * DEFAULT_NAME="global";
                //    static const std::string DEFAULT_NAME;
                //    ~Messenger();
                static void free();
                static void freeListener(std::string name = DEFAULT_NAME);
                static Messenger & getInstance(std::string name = DEFAULT_NAME);
                ~Messenger();
                void addMessageListener(MessageListener &listener, std::string name = DEFAULT_NAME);
                void removeMessageListener(MessageListener &listener, std::string name = DEFAULT_NAME);
                void sendMessage(Message & msg, std::string name = DEFAULT_NAME);
                void sendRequest(Message & msg, std::string name = DEFAULT_NAME);
            private:
                //    enum {DEFAULT= "global"};
                Messenger(std::string name);
                static std::map<std::string, Messenger*> messenger;
                static std::map<std::string, std::list<MessageListener*> > listener;
                //    static std::map<std::string,std::list<int> > listener;
            };

        }
    }
}
#endif
