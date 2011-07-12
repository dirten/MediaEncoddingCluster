/* 
 * File:   JsonProfileHandler.h
 * Author: HoelscJ
 *
 * Created on 20. Mai 2011, 11:35
 */

#ifndef JSONPROFILEHANDLER_H
#define	JSONPROFILEHANDLER_H
#include "mongoose.h"
#include "org/esb/libjson/libjson.h"


#include <string>
namespace org {
  namespace esb {
    namespace api {

      class JsonProfileHandler {
      public:
        static JSONNode handle(struct mg_connection *conn, const struct mg_request_info *request_info, db::HiveDb&, std::string postdata);
      private:
        JsonProfileHandler();
        virtual ~JsonProfileHandler();
        static bool contains(JSONNode& node, std::string name);
        static std::string checkJsonProfile(JSONNode&);
      };
    }
  }
}

#endif	/* JSONPROFILEHANDLER_H */

