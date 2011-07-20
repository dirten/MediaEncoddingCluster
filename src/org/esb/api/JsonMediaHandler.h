/* 
 * File:   JsonMediaHandler.h
 * Author: HoelscJ
 *
 * Created on 19. Juli 2011, 15:46
 */

#ifndef JSONMEDIAHANDLER_H
#define	JSONMEDIAHANDLER_H
#include "mongoose.h"
#include "org/esb/libjson/libjson.h"
#include <string>
namespace org {
  namespace esb {
    namespace api {

      class JsonMediaHandler {
      public:
        JsonMediaHandler();
        static JSONNode handle(struct mg_connection *conn, const struct mg_request_info *request_info, db::HiveDb &db, std::string postdata);
        virtual ~JsonMediaHandler();
      private:
        static bool contains(JSONNode& node, std::string name);
      };
    }
  }
}
#endif	/* JSONMEDIAHANDLER_H */

