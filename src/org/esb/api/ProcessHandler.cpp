/* 
 * File:   ProcessHandler.cpp
 * Author: HoelscJ
 * 
 * Created on 13. Juli 2011, 11:13
 */

#include "ProcessHandler.h"
#include "org/esb/util/Log.h"
namespace org {
  namespace esb {
    namespace api {

      ProcessHandler::ProcessHandler() {
      }

      ProcessHandler::~ProcessHandler() {
      }
      std::string ProcessHandler::getUrlToHandle(){
          return "/api/v1/process";
      }

      JSONNode ProcessHandler::handle(const struct mg_request_info *request_info, bool & status){
        std::string start=getParameter("start",request_info->query_string);
        LOGDEBUG("Start="+start);
        JSONNode result(JSON_NODE);

        result.set_name("ProcessHandler");
        result.push_back(JSONNode("processHandler","success"));
        status=true;
        return result;
      }
    }
  }
}
