/* 
 * File:   UrlHandler.h
 * Author: HoelscJ
 *
 * Created on 12. Juli 2011, 11:35
 */

#ifndef URLHANDLER_H
#define	URLHANDLER_H
#include "string"
#include "org/esb/libjson/libjson.h"
#include "mongoose.h"
namespace org {
  namespace esb {
    namespace api {

      class UrlHandler {
      public:

        virtual std::string getUrlToHandle() {
          return "";
        }

        virtual JSONNode handle(const struct mg_request_info *request_info, bool & status) {
			return JSONNode(JSON_NODE);
        };

        std::string getParameter(std::string key, char * query) {
          std::string result;
          if (query!=NULL) {
            char data[255];
            mg_get_var(query, strlen(query), key.c_str(), data, sizeof (data));
            result=data;
            //LOGDEBUG("DataId" << iddata);
          }
          return result;
        }

      };
    }
  }
}

#endif	/* URLHANDLER_H */

