#ifndef ORG_ESB_SIGNAL_MESSENGER_H
#define ORG_ESB_SIGNAL_MESSENGER_H
#include <string>
#include <map>
#include <list>
#include "MessageListener.h"
#define DEFAULT_NAME "global"
namespace org{
namespace esb{
namespace signal{
//class MessageListener;

class Messenger{
  public:
//    static const char * DEFAULT_NAME="global";
//    static const std::string DEFAULT_NAME;
//    ~Messenger();
    static void free();
    static Messenger & getInstance(std::string name=DEFAULT_NAME);
    void addMessageListener(MessageListener &listener, std::string name=DEFAULT_NAME);
    void removeMessageListener(MessageListener &listener, std::string name=DEFAULT_NAME);
    void sendMessage(Message & msg, std::string name=DEFAULT_NAME);
  private:
    Messenger(std::string name);
    static std::map<std::string,Messenger*> messenger;
    static std::map<std::string,std::list<MessageListener*> > listener;
//    static std::map<std::string,std::list<int> > listener;
};

}}}
#endif
