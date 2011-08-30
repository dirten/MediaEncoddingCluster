/*----------------------------------------------------------------------
 *  File    : ApiWebServer.cpp
 *  Author  : Jan Hölscher <jan.hoelscher@esblab.com>
 *  Purpose :
 *  Created : 12. Juli 2011 by Jan Hölscher <jan.hoelscher@esblab.com>
 *
 *
 * MediaEncodingCluster, Copyright (C) 2001-2011   Jan Hölscher
 *
 * This program License under the terms in the LICENSE file
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  
 *
 * ----------------------------------------------------------------------
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
      std::map<std::string,org::esb::core::WebservicePlugin *> ApiWebServer::_urlhandler;
        void ApiWebServer::startService(){
          if(port.length()==0)
            port="8080";
        const char *options[] = {
          "document_root", docroot.c_str(),
          "listening_ports", port.c_str(),
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
        void ApiWebServer::stopService(){
          
        }
        void ApiWebServer::setContext(org::esb::core::AppContext*c){
          LOGDEBUG("context parameter port:"<<c->env["web.port"]);
          LOGDEBUG("context parameter docroot:"<<c->env["web.docroot"]);
          port=c->env["web.port"];
          docroot=c->env["web.docroot"];
        }

      
      ApiWebServer::ApiWebServer() {
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
          

          if (_urlhandler.find(request) != _urlhandler.end()) {
            bool status=false;
            _urlhandler[request]->handle(NULL,NULL);
          }else{
            /*error header*/
          }
          processed=new char();
        }
        
        return processed;
      }

      bool ApiWebServer::addHandler(std::string url,org::esb::core::WebservicePlugin * handler) {
        
        _urlhandler[url] = handler;
		return true;
      }
      
      void ApiWebServer::addHook(std::string url,org::esb::core::HookPlugin * hook) {
        
      }
    }
  }
}
extern "C" void startApiServer() {
    org::esb::api::ApiWebServer api_server();
}
