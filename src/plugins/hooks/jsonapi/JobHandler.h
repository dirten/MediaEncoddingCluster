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
namespace jobhandler {

  class JSONAPI_EXPORT JobHandler : public org::esb::core::HookPlugin {
  public:
    JobHandler();
    virtual ~JobHandler();
    void handleRequest(org::esb::core::Request*, org::esb::core::Response*);
    org::esb::core::OptionsDescription getOptionsDescription();

    void init();
  private:
    void handleGET(org::esb::core::Request*, org::esb::core::Response*);
    void handlePOST(org::esb::core::Request*, org::esb::core::Response*);
    std::string _base_uri;
    bool contains(JSONNode& node, std::string name);
    std::string checkJob(JSONNode&root);
    std::string checkTask(JSONNode&root, db::Job & job);
    void save(JSONNode&root);
  };
  REGISTER_HOOK("web.api.Service", JobHandler, JobHandler::handleRequest, 10);
}
#endif	/* JOBHANDLER_H */

