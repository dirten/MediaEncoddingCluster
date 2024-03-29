/* 
 * File:   JSONHandler.h
 * Author: HoelscJ
 *
 * Created on 1. September 2011, 15:52
 */

#ifndef JSONHANDLER_H
#define	JSONHANDLER_H
#include "org/esb/db/hivedb.hpp"

#include "org/esb/core/HookPlugin.h"
#include "plugins/services/webservice/ServiceRequest.h"
#include "plugins/services/webservice/ServiceResponse.h"
#include "org/esb/libjson/JSONNode.h"
#include "org/esb/util/Log.h"
#include "exports.h"
#include <set>
using namespace org::esb::core;
using namespace org::esb::api;
namespace org {
  namespace esb {
    namespace plugin {

      class JSONAPI_EXPORT JSONHandler : public org::esb::core::HookPlugin {
        classlogger("org.esb.plugin.JSONHandler")
      public:
        JSONHandler();
        ~JSONHandler();
        void handleRequest(Request * req, Response*res);
        OptionsDescription getOptionsDescription();
        void init();
      private:
        std::string base_uri;
        boost::shared_ptr<db::HiveDb> _db;
        void handleEncoding(ServiceRequest * req, ServiceResponse*res);
        void handleProfile(ServiceRequest * req, ServiceResponse*res);
        void handleFormat(ServiceRequest * req, ServiceResponse*res);
        void handleCodec(ServiceRequest * req, ServiceResponse*res);
        std::set<std::string> valid_formats;
        std::set<std::string> valid_video_codecs;
        std::string checkJsonProfile(JSONNode&root);

        std::string checkJsonEncoding(JSONNode&root);
        bool contains(JSONNode& node, std::string name);
        JSONNode save(db::HiveDb&db, JSONNode & root);
        JSONNode save_outfile(db::HiveDb&db, JSONNode & root);
        JSONNode save_outdir(db::HiveDb&db, JSONNode & root);
      };
      REGISTER_HOOK("web.api.Service", JSONHandler, JSONHandler::handleRequest, 1);
    }
  }
}


#endif	/* JSONHANDLER_H */

