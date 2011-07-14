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
#include "boost/uuid/uuid_generators.hpp"
#include "boost/uuid/uuid_io.hpp"
#include "boost/lexical_cast.hpp"

namespace org {
  namespace esb {
    namespace api {
      std::map<std::string, UrlHandler*> ApiWebServer::_urlhandler;

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
        void *processed = NULL;
        if (event == MG_NEW_REQUEST) {
          static const char *reply_start =
                  "HTTP/1.1 200 OK\r\n"
                  "Cache: no-cache\r\n"
                  "Content-Type: text/plain; charset=utf-8\r\n"
                  "\r\n";
          std::string request = request_info->uri;
          boost::uuids::uuid uuid = boost::uuids::random_generator()();
          std::string requestId = boost::lexical_cast<std::string > (uuid);
          JSONNode n(JSON_NODE);

          if (_urlhandler.find(request) != _urlhandler.end()) {
            bool status=false;
            JSONNode node = _urlhandler[request]->handle(request_info, status);
            if(status){
              node.set_name("data");
              n.push_back(node);
            }else{
              node.set_name("error");
              n.push_back(node);
            }
          }else{
            JSONNode c(JSON_NODE);
            c.set_name("error");
            n.push_back(c);
          }
          n.push_back(JSONNode("requestId", requestId));
          std::string json_s = n.write();
          
          mg_write(conn, json_s.c_str(), json_s.length());
          processed=new char();
        }
        
        return processed;
      }

      bool ApiWebServer::addHandler(UrlHandler* handler) {
        std::string url = handler->getUrlToHandle();
        _urlhandler[url] = handler;
      }
    }
  }
}
extern "C" void startApiServer() {
    org::esb::api::ApiWebServer api_server(8888);
}
