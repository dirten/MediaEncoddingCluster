#ifndef ORG_ESB_SIGNAL_MESSAGELISTENER_H
#define ORG_ESB_SIGNAL_MESSAGELISTENER_H
#include "Message.h"
namespace org{
namespace esb{
namespace signal{
class SIGNAL_EXPORT MessageListener{
  public:
    virtual void onMessage(Message & msg)=0;
    virtual ~MessageListener(){};
};
}}}
#endif

