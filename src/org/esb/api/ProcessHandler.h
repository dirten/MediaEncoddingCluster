/* 
 * File:   ProcessHandler.h
 * Author: HoelscJ
 *
 * Created on 13. Juli 2011, 11:13
 */

#ifndef PROCESSHANDLER_H
#define	PROCESSHANDLER_H
#include "UrlHandler.h"
namespace org {
  namespace esb {
    namespace api {

      class ProcessHandler: public UrlHandler {
      public:
        ProcessHandler();
        virtual ~ProcessHandler();
        std::string getUrlToHandle();
        JSONNode handle(const struct mg_request_info *request_info,bool & status);
      private:

      };
    }
  }
}
#endif	/* PROCESSHANDLER_H */

