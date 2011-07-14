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
#include "org/esb/config/config.h"
#include "org/esb/io/File.h"
#include "org/esb/hive/PresetReader.h"
#include "org/esb/av/AV.h"
#include "boost/uuid/uuid_generators.hpp"
#include "boost/uuid/uuid_io.hpp"
#include "boost/lexical_cast.hpp"
#include "JsonProfileHandler.h"
#include "JsonEncodingHandler.h"
#include "JsonStatisticsHandler.h"
#include "boost/archive/iterators/base64_from_binary.hpp"
#include "boost/archive/iterators/binary_from_base64.hpp"
#include "boost/archive/iterators/transform_width.hpp"
#include "boost/thread/locks.hpp"
#include <string>
#include <iostream>
#define BASE_API_URL "/api/v1"
using namespace std;
using namespace boost::archive::iterators;

typedef
base64_from_binary<
transform_width<string::const_iterator, 6, 8 >
> base64_t;

typedef
transform_width<
binary_from_base64<string::const_iterator>, 8, 6
> binary_t;


namespace org {
  namespace esb {
    namespace api {
      //db::HiveDb * JsonServer::_db;
      std::set<std::string> JsonServer::valid_formats;

      boost::mutex JsonServer::http_mutex;

      JsonServer::JsonServer(int port) {
        std::string ports = org::esb::util::StringUtil::toString(port);
	std::string docroot=org::esb::config::Config::get("web.docroot");

        const char *options[] = {
          "document_root", docroot.c_str(),
          "listening_ports", ports.c_str(),
          "num_threads", "5",
          "index_files", "index.html",
          /*
          "protect_uri","/=test.file",
          "authentication_domain","localhost",
           */
          NULL
        };
        ctx = mg_start(&JsonServer::event_handler, NULL, options);
        assert(ctx != NULL);
        LOGDEBUG("Web server started on ports " << mg_get_option(ctx, "listening_ports"));
LOGDEBUG("Web server document root " << mg_get_option(ctx, "document_root"));
        //_db = org::esb::hive::DatabaseService::getDatabase();
        valid_formats.insert("amr");
        valid_formats.insert("asf");
        valid_formats.insert("avi");
        valid_formats.insert("avm2");
        valid_formats.insert("dv");
        valid_formats.insert("filmstrip");
        valid_formats.insert("flv");
        valid_formats.insert("gif");
        valid_formats.insert("ipod");
        valid_formats.insert("ivf");
        valid_formats.insert("m4v");
        valid_formats.insert("matroska");
        valid_formats.insert("mjpeg");
        valid_formats.insert("mov");
        valid_formats.insert("mp4");
        valid_formats.insert("mpeg");
        valid_formats.insert("mpegts");
        valid_formats.insert("vcd");
        valid_formats.insert("mpeg1video");
        valid_formats.insert("mpeg2video");
        valid_formats.insert("dvd");
        valid_formats.insert("vob");
        valid_formats.insert("svcd");
        valid_formats.insert("ogg");
        valid_formats.insert("psp");
        valid_formats.insert("rawvideo");
        valid_formats.insert("swf");
        valid_formats.insert("3g2");
        valid_formats.insert("3gp");
        valid_formats.insert("webm");
      }

      JsonServer::~JsonServer() {
        mg_stop(ctx);

      }

      bool JsonServer::contains(JSONNode& node, std::string name) {
        bool result = false;
        int size = node.size();
        LOGDEBUG("NodeSize=" << size);
        if (size > 0) {
          for (int a = 0; a < size; a++) {
            JSONNode n = node[a];
            LOGDEBUG("name=" << n.name());
            if (name == n.name()) {
              result = true;
            }
          }
        }
        return result;
      }

      void * JsonServer::event_handler(enum mg_event event,
              struct mg_connection *conn,
              const struct mg_request_info *request_info) {
        boost::mutex::scoped_lock scoped_lock(http_mutex);
        if (mg_modify_passwords_file("test.file", "localhost", "ich", "nich")) {
          //LOGDEBUG("entry created");
        }
        void *processed = new char();
        LOGDEBUG("URI=" << request_info->uri);
        LOGDEBUG("Method=" << request_info->request_method);
        //LOGDEBUG("HeaderCount:"<<request_info->num_headers);
        for (int a = 0; a < request_info->num_headers; a++) {
          //LOGDEBUG("Header"<<a<<" name:"<<request_info->http_headers[a].name);
          // LOGDEBUG("Header"<<a<<" value:"<<request_info->http_headers[a].value);
          /*
          if(strcmp(request_info->http_headers[a].name,"Authorization")==0){
            string str("test:jan");
            string enc1(base64_t(str.begin()), base64_t(str.end()));
            LOGDEBUG("encoded="<<enc1);
            std::string auth(request_info->http_headers[a].value);
            std:string enc(auth.begin()+6,auth.end()-1);
            LOGDEBUG("Try decode:"<<enc);
            string dec(binary_t(enc.begin()), binary_t(enc.end()));
            LOGDEBUG("Decoded:"<<dec);
          }*/
        }
        if (event == MG_NEW_REQUEST) {
          static const char *reply_start =
                  "HTTP/1.1 200 OK\r\n"
                  "Cache: no-cache\r\n"
                  "Content-Type: text/plain; charset=utf-8\r\n"
                  "\r\n";
          //_db.begin();
          db::HiveDb db=org::esb::hive::DatabaseService::getDatabase();

          boost::uuids::uuid uuid = boost::uuids::random_generator()();
          std::string requestId = boost::lexical_cast<std::string > (uuid);

          std::string postdata;
          if (strcmp(request_info->request_method, "POST") == 0) {
            /*reading the post data that comes in*/
            int bytes = 0, max = 150000;
            char buffer[1000];
            while ((bytes = mg_read(conn, buffer, sizeof (buffer))) > 0 && max > 0) {
              postdata = postdata.append(buffer, bytes);
              max -= bytes;
            }
          }
          LOGDEBUG("PostData:" << postdata);
          std::string request = request_info->uri;

          //db::Request req(_db);
          //req.requestId = requestId;
          if (request_info->query_string != NULL) {
            //req.query = std::string(request_info->query_string);
          }
          //req.uri = std::string(request_info->uri);
          //req.data = postdata;
          //req.requestType = std::string(request_info->request_method);
          /*only by api calls*/
          //if(request.find(BASE_API_URL"/profile")==0||request.find(BASE_API_URL"/encoding")==0)
          //req.update();

          //LOGDEBUG("Request=" << request);
          //LOGDEBUG("QueryString=" << request_info->query_string);
          LOGDEBUG("RequestMethod=" << request_info->request_method);
          if (request_info->query_string != NULL) {
            char iddata[100];
            mg_get_var(request_info->query_string, strlen(request_info->query_string), "id", iddata, sizeof (iddata));
            //LOGDEBUG("DataId" << iddata);
          }

          if (request == BASE_API_URL"/format") {
            mg_printf(conn, "%s", reply_start);
            JSONNode n(JSON_NODE);
            JSONNode c(JSON_ARRAY);
            c.set_name("data");
            AVOutputFormat *ofmt = NULL;
            int a = 0;
            while ((ofmt = av_oformat_next(ofmt))) {
              if (valid_formats.find(ofmt->name) != valid_formats.end()) {
                JSONNode cnode(JSON_NODE);
                cnode.push_back(JSONNode("longname", ofmt->long_name));
                cnode.push_back(JSONNode("name", ofmt->name));
                c.push_back(cnode);
              }
            }
            n.push_back(c);
            n.push_back(JSONNode("requestId", requestId));
            std::string json_s = n.write_formatted();
            //LOGDEBUG(json_s);
            mg_write(conn, json_s.c_str(), json_s.length());
            //mg_printf(conn, "%s", json_s.c_str());
            //req.response = json_s;

          } else if (request == BASE_API_URL"/codec") {
            mg_printf(conn, "%s", reply_start);
            JSONNode n(JSON_NODE);
            JSONNode c(JSON_ARRAY);
            c.set_name("data");

            AVCodec *p = NULL;
            int a = 0;
            while ((p = av_codec_next(p))) {
              if (p->encode && p->long_name != NULL) {
                JSONNode cnode(JSON_NODE);
                cnode.push_back(JSONNode("longname", p->long_name));
                cnode.push_back(JSONNode("id", p->name));
                cnode.push_back(JSONNode("type", p->type));
                c.push_back(cnode);
              }
            }
            n.push_back(c);
            n.push_back(JSONNode("requestId", requestId));
            std::string json_s = n.write_formatted();


            mg_write(conn, json_s.c_str(), json_s.length());
            //req.response = json_s;

          } else if (request == BASE_API_URL"/profile") {
            mg_printf(conn, "%s", reply_start);
            JSONNode n = JsonProfileHandler::handle(conn, request_info, db, postdata);
            n.push_back(JSONNode("requestId", requestId));
            std::string json_s = n.write_formatted();
            mg_write(conn, json_s.c_str(), json_s.length());
            //req.response = json_s;
            //req.update();

          } else if (request == BASE_API_URL"/encoding") {
            mg_printf(conn, "%s", reply_start);
            JSONNode n = JsonEncodingHandler::handle(conn, request_info, db, postdata);
            n.push_back(JSONNode("requestId", requestId));
            std::string json_s = n.write();
            mg_write(conn, json_s.c_str(), json_s.length());
            //req.response = json_s;
            //req.update();

          } else if (request == BASE_API_URL"/statistic") {
            mg_printf(conn, "%s", reply_start);
            JSONNode n = JsonStatisticsHandler::handle(conn, request_info, db, postdata);
            n.push_back(JSONNode("requestId", requestId));
            std::string json_s = n.write();
            mg_write(conn, json_s.c_str(), json_s.length());
            //req.response = json_s;
            //req.update();

          }else{
            //mg_printf(conn, "%s", request_info->uri);
            processed = NULL;
          }
          //LOGDEBUG("commit to db");
          //_db.commit();

        } else {
          processed = NULL;
        }

        return processed;
      }
    }
  }
}
