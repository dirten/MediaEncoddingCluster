/* 
 * File:   JsonEncodingHandler.h
 * Author: HoelscJ
 *
 * Created on 23. Mai 2011, 15:04
 */

#ifndef JSONENCODINGHANDLER_H
#define	JSONENCODINGHANDLER_H
#include "org/esb/hive/DatabaseService.h"
#include "mongoose.h"
#include "org/esb/libjson/libjson.h"
namespace org {
  namespace esb {
    namespace api {

      class JsonEncodingHandler {
      public:
        static JSONNode handle(struct mg_connection *conn, const struct mg_request_info *request_info, db::HiveDb&, std::string postdata);
      private:
        JsonEncodingHandler();
        virtual ~JsonEncodingHandler();
        static bool contains(JSONNode& node, std::string name);
        static std::string checkJsonProfile(JSONNode&);
        static JSONNode list(db::HiveDb&, bool);
        static JSONNode get(db::HiveDb&, std::string);
        static JSONNode save(db::HiveDb&, JSONNode & root);
        static JSONNode save_outfile(db::HiveDb&, JSONNode & root);
        static JSONNode save_outdir(db::HiveDb&, JSONNode & root);
        static JSONNode del(db::HiveDb&, std::string);
        static JSONNode stop(db::HiveDb&, std::string);
      };
    }
  }
}

#endif	/* JSONENCODINGHANDLER_H */

