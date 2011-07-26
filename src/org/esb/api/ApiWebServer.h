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
namespace org {
  namespace esb {
    namespace api {

      class ApiWebServer {
      public:
        ApiWebServer(int port);
        virtual ~ApiWebServer();
        bool addHandler(UrlHandler*);
      private:
        static void * event_handler(enum mg_event event, struct mg_connection *conn, const struct mg_request_info *request_info);
        struct mg_context *ctx;
        static std::map<std::string,UrlHandler*> _urlhandler;
      };
    }
  }
}

#endif	/* APIWEBSERVER_H */

