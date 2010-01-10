#ifndef ORG_ESB_WEB_WEBSERVER_H
#define ORG_ESB_WEB_WEBSERVER_H
#include "org/esb/signal/Message.h"
#include "org/esb/signal/MessageListener.h"
#include <Wt/WServer>

namespace org{
namespace esb{
namespace web{

    using namespace org::esb::signal;
    class WebServer: public MessageListener{
	public:
	    WebServer();
	    ~WebServer();
	    void onMessage(Message & msg);
		void start();
		void stop();
		void run();
	private:
		Wt::WServer server;
    };
}}}

#endif

