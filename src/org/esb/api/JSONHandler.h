/* 
 * File:   JSONHandler.h
 * Author: HoelscJ
 *
 * Created on 1. September 2011, 15:52
 */

#ifndef JSONHANDLER_H
#define	JSONHANDLER_H
#include "org/esb/core/HookPlugin.h"
#include "exports.h"
using namespace org::esb::core;
namespace db{
  class HiveDb;
}
namespace org {
  namespace esb {
    namespace api {
      class JSONHandler : public org::esb::core::HookPlugin {
      public:
        JSONHandler();
        virtual ~JSONHandler();
        void handleEncoding(Request * req, Response*res);
      private:
        std::string base_uri;        
        db::HiveDb * db;
      }jsonHandler;
      REGISTER_HOOK("web.api.Service", jsonHandler, JSONHandler::handleEncoding,1);
    }
  }
}

#endif	/* JSONHANDLER_H */

