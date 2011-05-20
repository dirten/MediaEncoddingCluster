/* 
 * File:   JsonServer.cpp
 * Author: HoelscJ
 * 
 * Created on 18. Mai 2011, 13:16
 */

#include "JsonServer.h"
#include "org/esb/util/StringUtil.h"
#include "org/esb/util/Log.h"
#include <string.h>
#include "JsonEncoding.h"
#include "org/esb/config/config.h"
#include "org/esb/io/File.h"
#include "org/esb/hive/PresetReader.h"
#include "org/esb/av/AV.h"
#include "boost/uuid/uuid_generators.hpp"
#include "boost/uuid/uuid_io.hpp"
#include "boost/lexical_cast.hpp"
#include "JsonProfileHandler.h"
namespace org {
  namespace esb {
    namespace api {
      db::HiveDb JsonServer::_db = org::esb::hive::DatabaseService::getDatabase();

      JsonServer::JsonServer(int port) {
        std::string ports = org::esb::util::StringUtil::toString(port);
        const char *options[] = {
          "document_root", (org::esb::config::Config::get("web.docroot")+"/www").c_str(),
          "listening_ports", ports.c_str(),
          "num_threads", "5",
          "index_files","mec.html",
          NULL
        };
        ctx = mg_start(&JsonServer::event_handler, NULL, options);
        assert(ctx != NULL);
        printf("Chat server started on ports %s, press enter to quit.\n",
                mg_get_option(ctx, "listening_ports"));


      }

      JsonServer::~JsonServer() {
        mg_stop(ctx);

      }
      bool JsonServer::contains(JSONNode& node, std::string name){
        bool result=false;
        int size=node.size();
        LOGDEBUG("NodeSize="<<size);
        if(size>0){
          for(int a=0;a<size;a++){
            JSONNode n=node[a];
            LOGDEBUG("name="<<n.name());
            if(name==n.name()){
              result = true;
            }
          }
        }
        return result;
      }
      void * JsonServer::event_handler(enum mg_event event,
              struct mg_connection *conn,
              const struct mg_request_info *request_info) {
        void *processed = new char();

        if (event == MG_NEW_REQUEST) {
          std::string request = request_info->uri;
          LOGDEBUG("Request=" << request);
          LOGDEBUG("QueryString=" << request_info->query_string);
          LOGDEBUG("RequestMethod=" << request_info->request_method);
          if (request_info->query_string != NULL) {
            char iddata[100];
            mg_get_var(request_info->query_string, strlen(request_info->query_string), "id", iddata, sizeof (iddata));
            LOGDEBUG("DataId" << iddata);
          }

          if (request == "/api/format") {
            JSONNode n(JSON_NODE);
            JSONNode c(JSON_ARRAY);
            c.set_name("data");
            AVOutputFormat *ofmt = NULL;
            int a = 0;
            while ((ofmt = av_oformat_next(ofmt))) {
              JSONNode cnode(JSON_NODE);
              cnode.push_back(JSONNode("longname", ofmt->long_name));
              cnode.push_back(JSONNode("name", ofmt->name));
              c.push_back(cnode);
            }
            n.push_back(c);
            std::string json_s = n.write();
            mg_printf(conn, "%s", json_s.c_str());

          } else if (request == "/api/codec") {
            JSONNode n(JSON_NODE);
            JSONNode c(JSON_ARRAY);
            c.set_name("data");

            AVCodec *p = NULL;
            int a = 0;
            while ((p = av_codec_next(p))) {
              if (p->encode && p->long_name != NULL) {
                JSONNode cnode(JSON_NODE);
                cnode.push_back(JSONNode("longname", p->long_name));
                cnode.push_back(JSONNode("id", p->id));
                cnode.push_back(JSONNode("type", p->type));
                c.push_back(cnode);
              }
            }
            n.push_back(c);
            std::string json_s = n.write_formatted();
            mg_write(conn, json_s.c_str(), json_s.length());

          } else if (request == "/api/profile") {
            JSONNode n =JsonProfileHandler::handle(conn,request_info, _db);
            std::string json_s = n.write();
            mg_write(conn, json_s.c_str(), json_s.length());

          } else if (request == "/api/encoding") {
            db::HiveDb db = org::esb::hive::DatabaseService::getDatabase();
            std::vector<db::Job> jobs = litesql::select<db::Job > (db).orderBy(db::Job::Id, false).all();
            JSONNode n(JSON_NODE);
            JSONNode c(JSON_ARRAY);
            c.set_name("data");
            std::vector<db::Job>::iterator jobit = jobs.begin();
            for (; jobit != jobs.end(); jobit++) {
              c.push_back(JsonEncoding((*jobit)));
            }
            n.push_back(c);
            std::string json_s = n.write();
            mg_printf(conn, "%s", json_s.c_str());
            mg_printf(conn, "\n");
          } else {
            //mg_printf(conn, "%s", request_info->uri);
            processed = NULL;
          }
        } else {
          processed = NULL;
        }
        return processed;
      }
    }
  }
}
