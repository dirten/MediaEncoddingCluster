#ifndef ORG_ESB_WEB_WEBSERVER_H
#define ORG_ESB_WEB_WEBSERVER_H

#include <stdlib.h>
#include "org/esb/lang/Runnable.h"
#include "org/esb/httpd/shttpd.h"

using namespace org::esb::lang;
namespace org{
	namespace esb{
		namespace web{
			class WebServer: public Runnable{
				public:
					WebServer(int port);
					void run();
					void stop();
				private:
					struct shttpd_ctx	*ctx;
					static bool _toStop;
			};
		}
	}
}
#endif

