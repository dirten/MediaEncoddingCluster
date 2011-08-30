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

      class  ApiWebServer:public org::esb::core::ServicePlugin, org::esb::core::HookProvider {
      public:
        API_EXPORT ApiWebServer();
        virtual API_EXPORT ~ApiWebServer();
        bool API_EXPORT addHandler(std::string url,org::esb::core::WebservicePlugin *);
        void API_EXPORT startService();
        void API_EXPORT stopService();
        void API_EXPORT setContext(org::esb::core::AppContext*);
        void API_EXPORT addHook(std::string hookname, org::esb::core::HookPlugin*);
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

