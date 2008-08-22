#ifndef ORG_ESB_WEB_WEBSERVER_H
#define ORG_ESB_WEB_WEBSERVER_H
#include "org/esb/lang/Runnable.h"
#include "org/esb/lang/Thread.h"
#include "org/esb/signal/Message.h"
#include "org/esb/signal/MessageListener.h"
namespace org{
namespace esb{
namespace web{
    using namespace org::esb::lang;
    using namespace org::esb::signal;
    class WebServer:public Runnable, MessageListener{
	public:
	    WebServer();
	    void run();
	    void onMessage(Message msg);
    };
}}}

#endif

