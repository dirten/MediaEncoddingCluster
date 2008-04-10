#ifndef ORG_ESB_WEB_WEBSERVER_H
#define ORG_ESB_WEB_WEBSERVER_H
#include "org/esb/lang/Runnable.h"
#include "org/esb/lang/Thread.h"

namespace org{
namespace esb{
namespace web{
    using namespace org::esb::lang;
    class WebServer:public Runnable{
	public:
	    WebServer();
	    void run();
    
    };
}}}

#endif

