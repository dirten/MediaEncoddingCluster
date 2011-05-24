/* 
 * File:   JsonEncodingHandler.cpp
 * Author: HoelscJ
 * 
 * Created on 23. Mai 2011, 15:04
 */

#include "JsonEncodingHandler.h"
#include "JsonEncoding.h"
#include "org/esb/hive/JobUtil.h"
#include "org/esb/hive/FileImporter.h"

namespace org {
  namespace esb {
    namespace api {

      JsonEncodingHandler::JsonEncodingHandler() {
      }

      bool JsonEncodingHandler::contains(JSONNode& node, std::string name) {
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

      std::string JsonEncodingHandler::checkJsonProfile(JSONNode&root) {
        std::string result;
        /*check the root conatins required data*/
        if (!contains(root, "infile")) {
          result = "no infile attribute given!";
        } else
          if (!contains(root, "profile")) {
          result = "no profile attribute found!";
        } else
          if (!contains(root, "outfile")) {
          result = "no outfile attribute found!";
        } 
        return result;
      }

      JSONNode JsonEncodingHandler::handle(struct mg_connection *conn, const struct mg_request_info *request_info, db::HiveDb& db) {
        std::string method(request_info->request_method);
        char iddata[100];
        memset(&iddata, 0, 100);
        if (request_info->query_string != NULL) {
          mg_get_var(request_info->query_string, strlen(request_info->query_string), "id", iddata, sizeof (iddata));
          LOGDEBUG("DataId" << iddata);
        }

        if (method == "DELETE") {
          return del(db, iddata);
        } else
          if (method == "GET") {
          if (strlen(iddata) > 0) {
            return get(db, iddata);
          } else {
            return list(db);
          }
        } else
          if (method == "POST") {
          JSONNode n(JSON_NODE);
          /*reading the post data that comes in*/
          int max_request=150000;
          int bytes = 0;
          char buffer[1000];
          std::string data;
          while ((bytes = mg_read(conn, buffer, sizeof (buffer))) > 0) {
            data = data.append(buffer, bytes);
            if(data.length()>max_request){
              JSONNode error(JSON_NODE);
              error.set_name("error");
              error.push_back(JSONNode("code", "request_to_large"));
              error.push_back(JSONNode("description", "Post request is to large"));
              n.push_back(error);
              return n;

            }
          }
          /*check if the incomming data is valid json data*/
          JSONNode inode;
          try {
            if (libjson::is_valid(data)) {
              LOGDEBUG("Data is valid");
              inode = libjson::parse(data);
              std::string msg = checkJsonProfile(inode);
              if (msg.length() > 0) {
                JSONNode error(JSON_NODE);
                error.set_name("error");
                error.push_back(JSONNode("code", "attribute_error"));
                error.push_back(JSONNode("description", msg));
                n.push_back(error);
                return n;
              }
              return save(db, inode);

            } else {
              JSONNode error(JSON_NODE);

              error.set_name("error");
              error.push_back(JSONNode("code", "parse_error"));
              error.push_back(JSONNode("description", "no valid json format given"));
              n.push_back(error);
              return n;
            }
          } catch (std::exception &ex) {
            LOGDEBUG(ex.what());
            JSONNode error(JSON_NODE);
            error.set_name("error");
            error.push_back(JSONNode("code", "parse_error"));
            error.push_back(JSONNode("description", "no valid json format given"));
            n.push_back(error);
            return n;
          }
        }

      }

      JSONNode JsonEncodingHandler::list(db::HiveDb& db) {
        LOGDEBUG("listing encodings");
        std::vector<db::Job> jobs = litesql::select<db::Job > (db).orderBy(db::Job::Id, false).all();
        JSONNode n(JSON_NODE);
        JSONNode c(JSON_ARRAY);
        c.set_name("data");
        std::vector<db::Job>::iterator jobit = jobs.begin();
        for (; jobit != jobs.end(); jobit++) {
          c.push_back(JsonEncoding((*jobit)));
        }
        n.push_back(c);
        return n;
      }

      JSONNode JsonEncodingHandler::get(db::HiveDb&db, std::string id) {
        JSONNode n(JSON_NODE);
        LOGDEBUG("loading encoding data for id " << id);
        litesql::DataSource<db::Job>s = litesql::select<db::Job > (db, db::Job::Id == id);
        if (s.count() > 0) {
    	  LOGDEBUG("Encoding found");
          db::Job job = s.one();
          n = JsonEncoding(job, false);
        } else {
          JSONNode error(JSON_NODE);
          error.set_name("error");
          error.push_back(JSONNode("code", "encoding_not_found"));
          error.push_back(JSONNode("description", "encoding not found"));
          n.push_back(error);
        }
        return n;
      }

      JSONNode JsonEncodingHandler::save(db::HiveDb&db, JSONNode & root) {
        JSONNode n(JSON_NODE);
        litesql::DataSource<db::Preset>s = litesql::select<db::Preset > (db, db::Preset::Uuid == root["profile"].as_string());
        if(s.count()==1){
          db::Preset preset=s.one();
          org::esb::hive::FileImporter importer;
          db::MediaFile infile=importer.import( org::esb::io::File(root["infile"].as_string()));
          
          int id=org::esb::hive::JobUtil::createJob(infile, preset, root["outfile"].as_string());
          n.push_back(JSONNode("bla",id));
        }else{
            JSONNode error(JSON_NODE);
            error.set_name("error");
            error.push_back(JSONNode("code", "encoding_create"));
            error.push_back(JSONNode("description", "create encoding failed"));
            n.push_back(error);
        }
        
        return n;
      }

      JSONNode JsonEncodingHandler::del(db::HiveDb&db, std::string id) {
        JSONNode n(JSON_NODE);
        if (id.length() > 0) {
          LOGDEBUG("loading preset data for id " << id);
          litesql::DataSource<db::Job>s = litesql::select<db::Job > (db, db::Job::Uuid == id);
          if (s.count() == 1) {
            db::Job job = s.one();
            job.del();
            JSONNode ok(JSON_NODE);
            ok.set_name("ok");
            ok.push_back(JSONNode("code", "encoding_deleted"));
            ok.push_back(JSONNode("description", "encoding succesful deleted"));
            n.push_back(ok);
          } else {
            JSONNode error(JSON_NODE);
            error.set_name("error");
            error.push_back(JSONNode("code", "encoding_not_found"));
            error.push_back(JSONNode("description", "encoding not found"));
            n.push_back(error);
          }
        } else {
          JSONNode error(JSON_NODE);
          error.set_name("error");
          error.push_back(JSONNode("code", "no_id"));
          error.push_back(JSONNode("description", "no id given for delete action"));
          n.push_back(error);
        }
        return n;
      }

      JsonEncodingHandler::~JsonEncodingHandler() {
      }
    }
  }
}
