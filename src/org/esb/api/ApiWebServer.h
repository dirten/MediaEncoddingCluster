/* 
 * File:   ApiWebServer.h
 * Author: HoelscJ
 *
 * Created on 12. Juli 2011, 11:29
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
        bool addHandler(UrlHandler&);
      private:
        static void * event_handler(enum mg_event event, struct mg_connection *conn, const struct mg_request_info *request_info);
        struct mg_context *ctx;
        std::map<std::string,UrlHandler> _handler;
      };
    }
  }
}

#endif	/* APIWEBSERVER_H */

