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
#include "org/esb/util/StringUtil.h"
#include "org/esb/signal/Message.h"
#include "org/esb/signal/Messenger.h"
#include "org/esb/util/StringTokenizer.h"

namespace org {
  namespace esb {
    namespace api {

      JsonEncodingHandler::JsonEncodingHandler() {
      }

      bool JsonEncodingHandler::contains(JSONNode& node, std::string name) {
        bool result = false;
        int size = node.size();
        //LOGDEBUG("NodeSize=" << size);
        if (size > 0) {
          for (int a = 0; a < size; a++) {
            JSONNode n = node[a];
            //LOGDEBUG("name=" << n.name());
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

      JSONNode JsonEncodingHandler::handle(struct mg_connection *conn, const struct mg_request_info *request_info, db::HiveDb& db, std::string postdata) {
        std::string method(request_info->request_method);
        char iddata[100];
        memset(&iddata, 0, 100);
        char stopdata[100];
        memset(&stopdata, 0, 100);
        char deldata[100];
        memset(&deldata, 0, 100);
        bool delflag = false;
        bool stopflag = false;
        bool fullflag=false;
        if (request_info->query_string != NULL) {
          mg_get_var(request_info->query_string, strlen(request_info->query_string), "id", iddata, sizeof (iddata));
          org::esb::util::StringTokenizer st(request_info->query_string, "&");
          while (st.hasMoreTokens()) {
            std::string tk = st.nextToken();
            if (tk == "stop") {
              stopflag = true;
              break;
            }
            if (tk == "delete") {
              delflag = true;
              break;
            }
            if (tk == "full") {
              fullflag = true;
              break;
            }
          }
          //delflag=mg_get_var(request_info->query_string, strlen(request_info->query_string), "delete", deldata, sizeof (deldata))!=-1;
          //stopflag=mg_get_var(request_info->query_string, strlen(request_info->query_string), "stop", stopdata, sizeof (stopdata))!=-1;
          //LOGDEBUG("DataId" << iddata);
        }

        if (method == "GET" && delflag) {
          return del(db, iddata);
        } else if (method == "GET" && stopflag) {
          return stop(db, iddata);
        } else
          if (method == "GET") {
          if (strlen(iddata) > 0) {
            return get(db, iddata);
          } else {
            return list(db, fullflag);
          }
        } else
          if (method == "POST" || method == "PUT") {
        JSONNode n(JSON_NODE);
          /*reading the post data that comes in*/
          /*
          
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
        }*/
          /*check if the incomming data is valid json data*/
          JSONNode inode;
          try {
            if (libjson::is_valid(postdata)) {
              LOGDEBUG("Data is valid");
              inode = libjson::parse(postdata);
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
            error.push_back(JSONNode("code", "internal_error"));
            error.push_back(JSONNode("description", "internal error ouccured"));
            n.push_back(error);
            return n;
          }
        }

      }

      JSONNode JsonEncodingHandler::list(db::HiveDb& db, bool full) {
        //LOGDEBUG("listing encodings");
        std::vector<db::Job> jobs = litesql::select<db::Job > (db).orderBy(db::Job::Id, false).all();
        JSONNode n(JSON_NODE);
        JSONNode c(JSON_ARRAY);
        c.set_name("data");
        std::vector<db::Job>::iterator jobit = jobs.begin();
        for (; jobit != jobs.end(); jobit++) {
          c.push_back(JsonEncoding((*jobit), !full));
        }
        n.push_back(c);
        return n;
      }

      JSONNode JsonEncodingHandler::get(db::HiveDb&db, std::string id) {
        JSONNode n(JSON_NODE);
        //LOGDEBUG("loading encoding data for id " << id);
        litesql::DataSource<db::Job>s = litesql::select<db::Job > (db, db::Job::Uuid == id);
        if (s.count() > 0) {
          //LOGDEBUG("Encoding found");
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
        if (s.count() == 1) {
          db::Preset preset = s.one();
          org::esb::hive::FileImporter importer;
          db::MediaFile infile = importer.import(org::esb::io::File(root["infile"].as_string()));
          if (infile.id > 0) {
            int id = org::esb::hive::JobUtil::createJob(infile, preset, root["outfile"].as_string());
            db::Job pre = litesql::select<db::Job > (db, db::Job::Id == id).one();

            n.push_back(JSONNode("id", pre.uuid.value()));
          } else {
            JSONNode error(JSON_NODE);
            error.set_name("error");
            error.push_back(JSONNode("code", "infile_not_found"));
            error.push_back(JSONNode("description", std::string("could not open input file with id ").append(root["infile"].as_string()).append(" for use to create encoding task!")));
            n.push_back(error);
          }
        } else {
          JSONNode error(JSON_NODE);
          error.set_name("error");
          error.push_back(JSONNode("code", "profile_not_found"));
          error.push_back(JSONNode("description", std::string("could not find profile with id ").append(root["profile"].as_string()).append(" for use to create encoding task!")));
          n.push_back(error);
        }

        return n;
      }

      JSONNode JsonEncodingHandler::del(db::HiveDb&db, std::string id) {
        JSONNode n(JSON_NODE);
        if (id.length() > 0) {
          LOGDEBUG("loading encoding data for id " << id);
          litesql::DataSource<db::Job>s = litesql::select<db::Job > (db, db::Job::Uuid == id);
          if (s.count() == 1) {
            db::Job job = s.one();
            if (job.status == "running") {
              JSONNode error(JSON_NODE);
              error.set_name("error");
              error.push_back(JSONNode("code", "is_running"));
              error.push_back(JSONNode("description", "encoding to stop, is currently running, please stop the encoding job before delete it!"));
              n.push_back(error);
            } else {
              //job.del();
              JSONNode ok(JSON_NODE);
              ok.set_name("ok");
              ok.push_back(JSONNode("code", "encoding_deleted"));
              ok.push_back(JSONNode("description", "encoding succesful deleted."));
              n.push_back(ok);
              job.del();
            }
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

      JSONNode JsonEncodingHandler::stop(db::HiveDb&db, std::string id) {
        JSONNode n(JSON_NODE);
        if (id.length() > 0) {
          //LOGDEBUG("loading encoding data for id " << id);
          litesql::DataSource<db::Job>s = litesql::select<db::Job > (db, db::Job::Uuid == id);
          if (s.count() == 1) {
            db::Job job = s.one();
            job.status = job.status == "running" ? "stopping" : "stopped";
            job.update();
            std::string job_id = org::esb::util::StringUtil::toString(job.id);
            org::esb::signal::Messenger::getInstance().sendMessage(org::esb::signal::Message().setProperty("processunitcontroller", "STOP_JOB").setProperty("job_id", job_id));

            //job.del();
            JSONNode ok(JSON_NODE);
            ok.set_name("ok");
            ok.push_back(JSONNode("code", "encoding_stopped"));
            ok.push_back(JSONNode("description", "stop encoding succesful signaled"));
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
