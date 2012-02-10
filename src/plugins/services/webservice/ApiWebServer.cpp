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
#include "org/esb/core/PluginContext.h"
#include "ApiWebServer.h"
#include "org/esb/util/StringUtil.h"
#include "org/esb/config/config.h"
#include "boost/uuid/uuid_generators.hpp"
#include "boost/uuid/uuid_io.hpp"
#include "boost/lexical_cast.hpp"
#include "ServiceRequest.h"
#include "ServiceResponse.h"


namespace org {
  namespace esb {
    namespace api {
      org::esb::core::HookNotificationCenter * ApiWebServer::center = NULL;
      char * ApiWebServer::p = new char();

      void ApiWebServer::startService() {
         
        LOGDEBUG("context parameter port:" << getContext()->getEnvironment<std::string>("webservice.port"));
        LOGDEBUG("context parameter docroot:" << getContext()->getEnvironment<std::string>("webservice.docroot"));
        port = getContext()->getEnvironment<std::string>("webservice.port");
        docroot = getContext()->getEnvironment<std::string>("webservice.docroot");

        if (port.length() == 0)
          port = "8080";
        const char *options[] = {
          "document_root", docroot.c_str(),
          "listening_ports", port.c_str(),
          "num_threads", "10",
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

      org::esb::core::ServicePlugin::ServiceType WEBSERVICE_EXPORT ApiWebServer::getServiceType() {
        return org::esb::core::ServicePlugin::SERVICE_TYPE_SERVER;
      }

      void ApiWebServer::stopService() {
        LOGTRACEMETHOD("ApiWebServer::stopService()")
        if (ctx)
          mg_stop(ctx);
      }

      org::esb::core::OptionsDescription ApiWebServer::getOptionsDescription() {
        org::esb::core::OptionsDescription result("webservice");
        result.add_options()
                ("webservice.port", boost::program_options::value<int >()->default_value(8080), "web server port listen on")
                ("webservice.docroot", boost::program_options::value<std::string > ()->default_value(org::esb::config::Config::get("web.docroot")), "web server document root")
                ("webservice.servicetype", boost::program_options::value<int > ()->default_value(1), "start this plugin on the Server/Client or both");
        return result;
      }

      ApiWebServer::ApiWebServer() {
        center = org::esb::core::HookNotificationCenter::getInstance();
        ctx = NULL;
      }

      ApiWebServer::~ApiWebServer() {
        LOGDEBUG("ApiWebServer::~ApiWebServer()");
        delete p;
        p=NULL;
      }

      void * ApiWebServer::event_handler(enum mg_event event,
              struct mg_connection *conn,
              const struct mg_request_info *request_info) {
        void *processed = NULL;
        if (event == MG_NEW_REQUEST) {
          ServiceResponse * res = new ServiceResponse(conn, request_info);
          ServiceRequest * req = new ServiceRequest(conn, request_info);
          center->postHook("web.api.Auth", req, res);
          if (res->_status != ServiceResponse::NONE) {
            return p;
          }
          center->postHook("web.api.PreProcess", req, res);
          
          center->postHook("web.api.Service", req, res);
          center->postHook("web.api.PostProcess", req, res);
          if (res->_status != ServiceResponse::NONE) {
            processed = p;
            res->flush();
          }
          delete res;
          delete req;
        }

        return processed;
      }
    }
  }
}


