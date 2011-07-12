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
namespace org {
  namespace esb {
    namespace api {

      class UrlHandler {
      public:
        std::string getUrlToHandle(){
          return "";
        }
        JSONNode handle(const struct mg_request_info *request_info);
      };
    }
  }
}

#endif	/* URLHANDLER_H */

