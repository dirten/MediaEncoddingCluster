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

#ifndef APIWEBSERVICE_H
#define	APIWEBSERVICE_H
#include "mongoose.h"
#include "org/esb/util/Log.h"
#include <map>
#include "org/esb/core/ServicePlugin.h"
#include "org/esb/core/HookNotificationCenter.h"
#include "exports.h"

namespace org {
  namespace esb {
    namespace api {

      class  ApiWebServer:public org::esb::core::ServicePlugin{
        classlogger("org.esb.plugin.ApiWebServer")
      public:
        WEBSERVICE_EXPORT ApiWebServer();
        virtual WEBSERVICE_EXPORT ~ApiWebServer();
        //bool API_EXPORT addHandler(std::string url,org::esb::core::WebservicePlugin *);
        void WEBSERVICE_EXPORT startService();
        void WEBSERVICE_EXPORT stopService();
        //void API_EXPORT addHook(std::string hookname, org::esb::core::HookPlugin*);
      private:
        static void * event_handler(enum mg_event event, struct mg_connection *conn, const struct mg_request_info *request_info);
        struct mg_context *ctx;
        std::string port;
        std::string docroot;
        static org::esb::core::HookNotificationCenter * center;
        static char * p;
      };
      REGISTER_SERVICE("apiwebserver", ApiWebServer)
      //REGISTER_HOOK_PROVIDER("web.api.url", ApiWebServer)
    }
  }
}
#endif	/* APIWEBSERVER_H */

