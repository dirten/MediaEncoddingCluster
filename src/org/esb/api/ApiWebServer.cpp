/* 
 * File:   ApiWebServer.cpp
 * Author: HoelscJ
 * 
 * Created on 12. Juli 2011, 11:29
 */

#include "ApiWebServer.h"
#include "org/esb/util/Log.h"
#include "org/esb/util/StringUtil.h"
#include "org/esb/config/config.h"
namespace org {
  namespace esb {
    namespace api {

      ApiWebServer::ApiWebServer(int port) {
        std::string ports = org::esb::util::StringUtil::toString(port);
        const char *options[] = {
          "document_root", (org::esb::config::Config::get("web.docroot")).c_str(),
          "listening_ports", ports.c_str(),
          "num_threads", "5",
          "index_files", "index.html",
          /*
          "protect_uri","/=test.file",
          "authentication_domain","localhost",
           */
          NULL
        };
        ctx = mg_start(&ApiWebServer::event_handler, NULL, options);
        assert(ctx != NULL);
        LOGDEBUG("Web server started on ports " << mg_get_option(ctx, "listening_ports"));
      }

      ApiWebServer::~ApiWebServer() {
      }
      void * ApiWebServer::event_handler(enum mg_event event,
              struct mg_connection *conn,
              const struct mg_request_info *request_info) {
        void *processed = new char();
        if (event == MG_NEW_REQUEST) {
          static const char *reply_start =
                  "HTTP/1.1 200 OK\r\n"
                  "Cache: no-cache\r\n"
                  "Content-Type: text/plain; charset=utf-8\r\n"
                  "\r\n";
        }
        processed=NULL;
        return processed;
      }

      bool ApiWebServer::addHandler(UrlHandler& handler) {
        std::string url=handler.getUrlToHandle();
        _handler[url]=handler;
      }
    }
  }
}
