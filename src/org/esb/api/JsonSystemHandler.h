/* 
 * File:   JsonStatisticsHandler.h
 * Author: HoelscJ
 *
 * Created on 7. Juli 2011, 13:55
 */

#ifndef JSONSSYSTEMHANDLER_H
#define	JSONSSYSTEMHANDLER_H
#include "org/esb/hive/DatabaseService.h"
#include "mongoose.h"
#include "org/esb/libjson/libjson.h"

namespace org {
  namespace esb {
    namespace api {

      class JsonSystemHandler {
      public:
        JsonSystemHandler();
        virtual ~JsonSystemHandler();
        static JSONNode handle(struct mg_connection *conn, const struct mg_request_info *request_info, db::HiveDb& db, std::string postdata);
      private:

      };
    }
  }
}
#endif	/* JSONSTATISTICSHANDLER_H */

