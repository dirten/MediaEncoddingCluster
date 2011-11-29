/* 
 * File:   JobHandler.h
 * Author: HoelscJ
 *
 * Created on 21. Oktober 2011, 14:18
 */

#ifndef JOBHANDLER_H
#define	JOBHANDLER_H
#include "org/esb/db/hivedb.hpp"

#include "org/esb/core/HookPlugin.h"
#include "org/esb/core/Request.h"
#include "org/esb/core/Response.h"
#include "org/esb/libjson/JSONNode.h"

#include "exports.h"
namespace graphhandler {

  class JSONAPI_EXPORT GraphHandler : public org::esb::core::HookPlugin {
    classlogger("jsonapi.GraphHandler")
  public:
    GraphHandler();
    virtual ~GraphHandler();
    void handleRequest(org::esb::core::Request*, org::esb::core::Response*);
    org::esb::core::OptionsDescription getOptionsDescription();

    void init();
  private:
    void handleGET(org::esb::core::Request*, org::esb::core::Response*);
    void handleDELETE(org::esb::core::Request*, org::esb::core::Response*);
    void handlePOST(org::esb::core::Request*, org::esb::core::Response*);
    bool contains(JSONNode& node, std::string name);
    void save(JSONNode&root, std::string & uuid);
    std::string _base_uri;
  };
  REGISTER_HOOK("web.api.Service", GraphHandler, GraphHandler::handleRequest, 11);
}
#endif	/* JOBHANDLER_H */

