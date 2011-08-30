/*----------------------------------------------------------------------
 *  File    : ApiWebServer.h
 *  Author  : Jan Hölscher <jan.hoelscher@esblab.com>
 *  Purpose :
 *  Created : 12. Juli 2011 by Jan Hölscher <jan.hoelscher@esblab.com>
 *
 *
 * MediaEncodingCluster, Copyright (C) 2001-2009   Jan Hölscher
 *
 * This program License under the terms in the LICENSE file
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  
 *
 * ----------------------------------------------------------------------
 */

#ifndef APIWEBSERVER_H
#define	APIWEBSERVER_H
#include "mongoose.h"
#include "UrlHandler.h"
#include <map>
#include "org/esb/core/ServicePlugin.h"
#include "org/esb/core/WebservicePlugin.h"
#include "org/esb/core/HookProvider.h"
#include "exports.h"
namespace org {
  namespace esb {
    namespace api {

      class API_EXPORT ApiWebServer:public org::esb::core::ServicePlugin, org::esb::core::HookProvider {
      public:
        ApiWebServer();
        virtual ~ApiWebServer();
        bool addHandler(std::string url,org::esb::core::WebservicePlugin *);
        void startService();
        void stopService();
        void setContext(org::esb::core::AppContext*);
        void addHook(std::string hookname, org::esb::core::HookPlugin*);
      private:
        static void * event_handler(enum mg_event event, struct mg_connection *conn, const struct mg_request_info *request_info);
        struct mg_context *ctx;
        static std::map<std::string,org::esb::core::WebservicePlugin *> _urlhandler;
        std::string port;
        std::string docroot;
      };
      REGISTER_SERVICE("apiwebserver", ApiWebServer)
    }
  }
}
#endif	/* APIWEBSERVER_H */

