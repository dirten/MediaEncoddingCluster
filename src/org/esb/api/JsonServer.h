/* 
 * File:   JsonServer.h
 * Author: HoelscJ
 *
 * Created on 18. Mai 2011, 13:16
 */

#ifndef JSONSERVER_H
#define	JSONSERVER_H
#include "org/esb/hive/DatabaseService.h"
#include "mongoose.h"
#include "org/esb/libjson/libjson.h"
#include "boost/thread/mutex.hpp"
#include <set>
#include "exports.h"
namespace org {
  namespace esb {
    namespace api {

      class API_EXPORT JsonServer {
      public:
        JsonServer(int port);
        virtual ~JsonServer();
      private:
        static void * event_handler(enum mg_event event, struct mg_connection *conn, const struct mg_request_info *request_info);
        struct mg_context *ctx;
        static bool contains(JSONNode&, std::string);
        //static db::HiveDb * _db;
        static boost::mutex http_mutex;
          static std::set<std::string> valid_formats;
          static std::set<std::string> valid_video_codecs;
      };
    }
  }
}

#endif	/* JSONSERVER_H */

