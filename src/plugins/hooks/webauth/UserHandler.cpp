/* 
 * File:   UserHandler.cpp
 * Author: HoelscJ
 * 
 * Created on 11. Oktober 2011, 11:02
 */

#include "UserHandler.h"
#include "plugins/services/webservice/ServiceRequest.h"
#include "plugins/services/webservice/ServiceResponse.h"
#include "org/esb/db/hivedb.hpp"
#include "org/esb/core/PluginContext.h"
namespace webauth {
  using namespace org::esb::api;

  UserHandler::UserHandler() {
  }

  UserHandler::~UserHandler() {
  }

  void UserHandler::handleRequest(Request * req, Response*res) {
    ServiceRequest * sreq = static_cast<ServiceRequest*> (req);
    ServiceResponse * sres = static_cast<ServiceResponse*> (res);
    if (sreq->getMethod() == "GET") {
      std::string username = sreq->getParameter("user");
      std::string passwd = sreq->getParameter("passwd");
      if (sreq->hasHeader("Authentication-Token")) {
        std::string token = sreq->getHeader("Authentication-Token");
        if (_user_map.count(token) > 0) {
          sres->setStatus(ServiceResponse::OK);
          sres->getOutputStream()->setHeader("Authentication-Token", token);
          return;
        }
      }
      if (username.length() > 0 && passwd.length() > 0) {
        litesql::DataSource<db::User> userlist = litesql::select<db::User > (*getContext()->database, db::User::Authname == username && db::User::Authpass == passwd);
        if (userlist.count() == 1) {
          sres->setStatus(ServiceResponse::OK);
          sres->getOutputStream()->setHeader("Authentication-Token", sreq->getUUID());
          return;
        }
      }
      sres->setStatus(ServiceResponse::FORBIDDEN);
      sres->getOutputStream()->write("not authorized");
    }
  }

  OptionsDescription UserHandler::getOptionsDescription() {
    org::esb::core::OptionsDescription result("webauth");
    result.add_options()
            ("webauth.protecturi", boost::program_options::value<std::string > ()->default_value("/api/v1"), "protected uri for the api requests");
    return result;

  }

  void UserHandler::init() {

  }

}
