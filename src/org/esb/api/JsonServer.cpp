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
namespace org {
  namespace esb {
    namespace api {
      db::HiveDb JsonServer::_db = org::esb::hive::DatabaseService::getDatabase();

      JsonServer::JsonServer(int port) {
        std::string ports = org::esb::util::StringUtil::toString(port);
        const char *options[] = {
          "document_root", org::esb::config::Config::get("web.docroot").c_str(),
          "listening_ports", ports.c_str(),
          "num_threads", "5",
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
            std::string json_s = n.write();
            mg_printf(conn, "%s", json_s.c_str());

          } else if (request == "/api/profile") {
            JSONNode n(JSON_NODE);
            if (strcmp(request_info->request_method, "GET") == 0) {
              org::esb::io::File presetdir(org::esb::config::Config::get("preset.path"));
              JSONNode c(JSON_ARRAY);
              c.set_name("data");
              if (presetdir.exists()) {
                org::esb::io::FileList files = presetdir.listFiles();
                std::list<std::list<std::string> > data;
                org::esb::io::FileList::iterator file_it = files.begin();
                for (; file_it != files.end(); file_it++) {
                  org::esb::hive::PresetReader reader((*file_it)->getPath());
                  LOGDEBUG((*file_it)->getPath());
                  std::list<std::string> line;
                  JSONNode prnode(JSON_NODE);
                  prnode.push_back(JSONNode("filename", (*file_it)->getFileName()));
                  prnode.push_back(JSONNode("profilename", reader.getPreset()["name"]));
                  c.push_back(prnode);
                }
              }
              n.push_back(c);
            }
            /*
             * when the preset will be updated
             *
             */
            else if (strcmp(request_info->request_method, "POST") == 0){
                            int bytes=0;
              char buffer[1000];
              std::string data;
              while((bytes=mg_read(conn, buffer, sizeof(buffer)))>0){
                data=data.append(buffer, bytes);
              }
              boost::uuids::uuid uuid=boost::uuids::random_generator()();
              std::string uuidstr=boost::lexical_cast<std::string>(uuid);

              if (request_info->query_string != NULL) {
                char iddata[100];
                mg_get_var(request_info->query_string, strlen(request_info->query_string), "id", iddata, sizeof (iddata));
                LOGDEBUG("DataId" << iddata);
                _db.begin();
                LOGDEBUG("DATABASE open")
                vector<db::Preset> presets=litesql::select<db::Preset>(_db, db::Preset::Uuid==iddata).all();
                LOGDEBUG("selection get")
                if(presets.size()==1){
                  uuidstr=iddata;
                  db::Preset preset=presets.front();
                  preset.data=data;
                  preset.update();
                }else{
                  db::Preset preset(_db);
                  preset.data=data;
                  preset.uuid=uuidstr;
                  preset.update();
                }
                _db.commit();
                LOGDEBUG("preset updated");
              }
              
              LOGDEBUG(data);
              try{
                if(libjson::is_valid(data)){
                  LOGDEBUG("Data is valid");
                JSONNode inode=libjson::parse(data);

                LOGDEBUG("INodeType:"<<(inode.type()==JSON_NODE));
                //LOGDEBUG("INode size:"<<inode.size());
                if(!contains(inode, "name")){
                  LOGDEBUG("name does not exit");
                }
                LOGDEBUG(uuidstr);
                JSONNode id("id",uuidstr);
                inode.push_back(id);
                n=inode;
                }else{
                  JSONNode error(JSON_NODE);


                  error.set_name("error");
                  error.push_back(JSONNode("code","parse_error"));
                  error.push_back(JSONNode("description","no valid json format given"));
                  n.push_back(error);

                }
              }catch(std::exception &ex){
                LOGDEBUG(ex.what());

                
                n.empty();
                n.set_name("error");
                n.push_back(JSONNode("code",1));
                n.push_back(JSONNode("description","no valid json format given"));
              }
            }
            std::string json_s = n.write();
            mg_printf(conn, "%s", json_s.c_str());

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
