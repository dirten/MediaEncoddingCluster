/* 
 * File:   JsonServer.h
 * Author: HoelscJ
 *
 * Created on 18. Mai 2011, 13:16
 */

#ifndef JSONSERVER_H
#define	JSONSERVER_H
#include "org/esb/hive/mongoose.h"
#include "libjson/libjson.h"

namespace org {
  namespace esb {
    namespace api {

      class JsonServer {
      public:
        JsonServer(int port);
        virtual ~JsonServer();
      private:
        static void * event_handler(enum mg_event event, struct mg_connection *conn, const struct mg_request_info *request_info);
        struct mg_context *ctx;
        static bool contains(JSONNode&, std::string);
      };
    }
  }
}

#endif	/* JSONSERVER_H */

