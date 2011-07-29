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
#include "org/esb/util/StringUtil.h"

#include "org/esb/io/FileFilter.h"

namespace org {
  namespace esb {
    namespace api {

      class MyFileFilter : public org::esb::io::FileFilter {
      public:

        MyFileFilter(std::string ext) {
          //          logdebug("extensions"<<ext);
          org::esb::util::StringTokenizer tokenizer(ext, ",");
          while (tokenizer.hasMoreTokens()) {
            std::string tok = tokenizer.nextToken();
            tok = org::esb::util::StringUtil::trim(tok);
            if (tok.length() > 0) {
              std::string e = ".";
              e += tok;
              media_ext[e];
              //logdebug("Extension added:"<<e);
            }
          }
        }

        bool accept(org::esb::io::File file) {
          bool result = false;
          if (file.isDirectory() || media_ext.size() == 0 || media_ext.find(file.getExtension()) != media_ext.end())
            result = true;
          return result;
        }
      private:
        map<std::string, std::string> media_ext;
      };

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
        /*check the root contains required data*/
        if (contains(root, "infile")) {
          if (!contains(root, "profileid")) {
            result = "no profileid attribute found!";
          } else if (!contains(root, "outfile")) {
            result = "no outfile attribute found!";
          }
        } else if (contains(root, "indir")) {
          if (!contains(root, "profileid")) {
            result = "no profileid attribute found!";
          } else if (!contains(root, "outdir")) {
            result = "no outfile attribute found!";
          } else if (!contains(root, "outfilepattern")) {
            result = "no outfilepattern attribute found!";
          }
        } else {
          result = "no infile or indir attribute given!";
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
        bool fullflag = false;
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
        if (contains(root, "outfile")) {
          return save_outfile(db, root);
        } else if (contains(root, "outdir")) {
          return save_outdir(db, root);
        }
      }

      JSONNode JsonEncodingHandler::save_outdir(db::HiveDb&db, JSONNode & root) {
        JSONNode n(JSON_NODE);
        org::esb::io::File indir(root["indir"].as_string());
        org::esb::io::File outdir(root["outdir"].as_string());
        if (!indir.exists() || !indir.isDirectory()) {
          JSONNode error(JSON_NODE);
          error.set_name("error");
          error.push_back(JSONNode("code", "indir_not_readable"));
          error.push_back(JSONNode("description", std::string("does not exist or is not a directory ").append(root["indir"].as_string()).append(" for use to create encoding tasks!")));
          n.push_back(error);
          return n;
        }
        if (!outdir.exists() || !outdir.isDirectory()) {
          JSONNode error(JSON_NODE);
          error.set_name("error");
          error.push_back(JSONNode("code", "outdir_not_writeable"));
          error.push_back(JSONNode("description", std::string("could not write to output directory ").append(root["outdir"].as_string()).append(" for use to create encoding tasks!")));
          n.push_back(error);
          return n;
        }

        litesql::DataSource<db::Preset>s = litesql::select<db::Preset > (db, db::Preset::Uuid == root["profileid"].as_string());
        if (s.count() == 1) {
          db::Preset preset = s.one();
          org::esb::hive::FileImporter importer;
          std::string filter;
          if (contains(root, "filter")) {
            filter = root["filter"].as_string();
          }
          std::string prio;
          if (contains(root, "priority")) {
            prio = root["priority"].as_string();
          }
          MyFileFilter ffilter = MyFileFilter(filter);
          org::esb::io::FileList list = org::esb::io::File(root["indir"].as_string()).listFiles(ffilter);
          org::esb::io::FileList::iterator it = list.begin();
          JSONNode ids(JSON_ARRAY);
          ids.set_name("data");
          JSONNode errors(JSON_ARRAY);
          errors.set_name("errors");
          for (; it != list.end(); it++) {
            org::esb::io::File ifile = org::esb::io::File((*it)->getPath());
            std::string outfile = root["outdir"].as_string();
            outfile += "/" + root["outfilepattern"].as_string();
            std::string ifilename = ifile.getFileName();
            ifilename = org::esb::util::StringUtil::replace(ifilename, ifile.getExtension(), "");
            std::string ext = org::esb::util::StringUtil::replace(ifile.getExtension(), ".", "");

            /*replace the markers in the file pattern*/
            outfile = org::esb::util::StringUtil::replace(outfile, "$filename", ifilename);
            outfile = org::esb::util::StringUtil::replace(outfile, "$extension", ext);
            outfile = org::esb::util::StringUtil::replace(outfile, "$profilename", preset.name);
            outfile = org::esb::util::StringUtil::replace(outfile, "$profileid", preset.uuid);
            /*replace some special chars in the filenames*/
            //outfile = org::esb::util::StringUtil::replace(outfile, ".", "_");
            outfile = org::esb::util::StringUtil::replace(outfile, " ", "_");
            

            if(org::esb::io::File(outfile).canWrite()){
              db::MediaFile infile = importer.import(ifile);
              if (infile.id > 0) {
                int id = org::esb::hive::JobUtil::createJob(infile, preset, outfile);
                db::Job pre = litesql::select<db::Job > (db, db::Job::Id == id).one();
                ids.push_back(JSONNode("id", pre.uuid.value()));
              } else {
                JSONNode error(JSON_NODE);
                error.set_name("error");
                error.push_back(JSONNode("code", "infile_not_found"));
                error.push_back(JSONNode("description", std::string("could not open input file with id ").append(ifile.getPath()).append(" for use to create encoding task!")));
                errors.push_back(error);
              }
            }else{
                JSONNode error(JSON_NODE);
                error.set_name("error");
                error.push_back(JSONNode("code", "outfile_not_writable"));
                error.push_back(JSONNode("description", std::string("could not write output file ").append(outfile).append(" for use to create encoding task!")));
                errors.push_back(error);
            }
          }
          if(errors.size()>0)
            n.push_back(errors);
          n.push_back(ids);
        } else {
          JSONNode error(JSON_NODE);
          error.set_name("error");
          error.push_back(JSONNode("code", "profile_not_found"));
          error.push_back(JSONNode("description", std::string("could not find profile with id ").append(root["profileid"].as_string()).append(" for use to create encoding task!")));
          n.push_back(error);
        }
        return n;
      }

      JSONNode JsonEncodingHandler::save_outfile(db::HiveDb&db, JSONNode & root) {
        JSONNode n(JSON_NODE);
        org::esb::io::File outfile(root["outfile"].as_string());
        if (!outfile.canWrite()) {
          JSONNode error(JSON_NODE);
          error.set_name("error");
          error.push_back(JSONNode("code", "outfile_not_writable"));
          error.push_back(JSONNode("description", std::string("could not write output file ").append(root["outfile"].as_string()).append(" for use to create encoding task!")));
          n.push_back(error);
        } else {
          litesql::DataSource<db::Preset>s = litesql::select<db::Preset > (db, db::Preset::Uuid == root["profileid"].as_string());
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
            error.push_back(JSONNode("description", std::string("could not find profile with id ").append(root["profileid"].as_string()).append(" for use to create encoding task!")));
            n.push_back(error);
          }
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
              error.push_back(JSONNode("description", "encoding to delete is currently running, please stop the encoding job before delete it!"));
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
