/* 
 * File:   JSONHandler.cpp
 * Author: HoelscJ
 * 
 * Created on 1. September 2011, 15:52
 */

#include "JSONHandler.h"

#include "org/esb/util/Log.h"
#include "org/esb/util/StringUtil.h"
#include "org/esb/util/StringTokenizer.h"
#include "org/esb/io/File.h"
#include "org/esb/av/AV.h"
#include "org/esb/av/FormatBaseStream.h"
#include "org/esb/libjson/libjson.h"
#include "org/esb/util/Foreach.h"
#include "org/esb/config/config.h"
#include "boost/uuid/uuid_generators.hpp"
#include "boost/uuid/uuid_io.hpp"
#include "boost/lexical_cast.hpp"
#include "org/esb/signal/Message.h"
#include "org/esb/signal/Messenger.h"
#include "PresetVerifier.h"
//#include "org/esb/hive/FileImporter.h"
//#include "org/esb/hive/JobUtil.h"
namespace org {
  namespace esb {
    namespace plugin {
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

      class JsonEncoding:public JSONNode{
      public:
      JsonEncoding(std::string json_string):JSONNode(JSON_NODE) { }
      JsonEncoding(db::Job job, bool simple):JSONNode(JSON_NODE) {
        push_back(JSONNode("id",job.uuid.value()));
        if(!simple){
          push_back(JSONNode("created",job.created));
          push_back(JSONNode("begintime",job.begintime));
          push_back(JSONNode("endtime",job.endtime));
          push_back(JSONNode("progress",job.progress.value()));
          push_back(JSONNode("fps",job.fps.value()));
          push_back(JSONNode("infile",job.infile.value()));
          push_back(JSONNode("outfile",job.outfile.value()));
          push_back(JSONNode("status",job.status.value()));
          std::string profileid;
          if (libjson::is_valid(job.data.value())) {
            JSONNode profile = libjson::parse(job.data.value());
            profile.set_name("profile");
            push_back(profile);
            //profileid=profile["id"].as_string();
          }
          db::Preset preset=job.preset().get().one();
          push_back(JSONNode("profileid",preset.uuid));
        }
      }

      ~JsonEncoding() {
      }
      };

      JSONHandler::JSONHandler() {
		  std::cout << "Constructor start"<<std::endl;
        base_uri = "/api/v1";
        //LOGDEBUG("JSONHandler::JSONHandler()");
        db = new db::HiveDb("sqlite3", org::esb::config::Config::get("db.url"));
        //org::esb::av::FormatBaseStream::initialize();

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
		  std::cout << "Constructor end"<<std::endl;

      }

      JSONHandler::~JSONHandler() {
        //LOGDEBUG("JSONHandler::~JSONHandler()");
        delete db;
      }
      bool JSONHandler::contains(JSONNode& node, std::string name) {
        bool result = false;
        int size = node.size();
        //LOGDEBUG("NodeSize=" << size);
        if (size > 0) {
          for (int a = 0; a < size; a++) {
            JSONNode n = node[a];
            LOGDEBUG("search for "<<name<<" iter = " << n.name());
            if (name == n.name()) {
              result = true;
              LOGDEBUG("attribute "<<name<<" found")
            }
          }
        }
        return result;
      }
      std::string JSONHandler::checkJsonEncoding(JSONNode&root) {
        std::string result;
        /*check the root contains required data*/
        if (contains(root, "infile")) {
          if (!contains(root, "profileid")) {
            return "no profileid attribute found!";
          } else if (!contains(root, "outfile")) {
            return "no outfile attribute found!";
          }
        } else if (contains(root, "indir")) {
          if (!contains(root, "profileid")) {
            return "no profileid attribute found!";
          } else if (!contains(root, "outdir")) {
            return "no outfile attribute found!";
          } else if (!contains(root, "outfilepattern")) {
            return "no outfilepattern attribute found!";
          }
        } else {
          return "no infile or indir attribute given!";
        }
        return "";
      }

      std::string JSONHandler::checkJsonProfile(JSONNode&root) {
        std::string result;
        /*check the root conatins required data*/
        if (!contains(root, "name")) {
          result = "no profile name given!";
        } else
          if (!contains(root, "format")) {
          result = "no format attribute found!";
        } else
          if (!contains(root, "video")) {
          result = "no video attribute found!";
        } else
          if (!contains(root, "audio")) {
          result = "no audio attribute found!";
        }else
          if (!contains(root["format"], "id")) {
          result = "no id attribute found in attribute \"format\"!";
        }else
          if (!contains(root["video"], "id")) {
          result = "no id attribute found in attribute \"video\"!";
        }else
          if (!contains(root["audio"], "id")) {
          result = "no id attribute found in attribute \"audio\"!";
        }
        result=PresetVerifier::verify(root);
        return result;
      }

      void JSONHandler::handleRequest(Request * req, Response*res) {
        ServiceRequest*sreq = ((ServiceRequest*) req);
        if (sreq->getRequestURI().find(base_uri + "/encoding") == 0) {
          handleEncoding(sreq, (ServiceResponse*) res);
        } else if (sreq->getRequestURI().find(base_uri + "/profile") == 0) {
          handleProfile(sreq, (ServiceResponse*) res);
        } else if (sreq->getRequestURI().find(base_uri + "/format") == 0) {
          handleFormat(sreq, (ServiceResponse*) res);
        } else if (sreq->getRequestURI().find(base_uri + "/codec") == 0) {
          handleCodec(sreq, (ServiceResponse*) res);
        } else {
          /*
          std::vector<db::Job> jobs = litesql::select<db::Job > (*db, db::Job::Status != "deleted").orderBy(db::Job::Id, false).all();
          JSONNode n(JSON_NODE);
          JSONNode c(JSON_ARRAY);
          c.set_name("data");
          std::vector<db::Job>::iterator jobit = jobs.begin();
          for (; jobit != jobs.end(); jobit++) {
            c.push_back(JSONNode("id", (*jobit).uuid.value()));
            //c.push_back(JsonEncoding((*jobit), false));
          }
          n.push_back(c);
           */
          //ServiceResponse*sres = ((ServiceResponse*) res);
          //sres->setStatus(404);
          //sres->getOutputStream()->write(n.write());
        }
      }
      int charcounter=0;
      int counter=0;
      bool first_open_found=false;
      bool jsonFilter(std::string data){
        if(counter==0&&first_open_found){
          return false;
        }
        charcounter++;
        if(data=="{"){
          counter++;
          first_open_found=true;
        }
        if(data=="}"){
          counter--;
        }
        return true;
      }
      void JSONHandler::handleEncoding(ServiceRequest* req, ServiceResponse* res) {
            JSONNode n(JSON_NODE);
        if (req->getMethod() == "POST") {
          JSONNode inode;
          try {
            std::string postdata;
            req->getInputstream()->read(postdata, &jsonFilter,1);
            LOGDEBUG("charcounter="<<charcounter<< " counter="<<counter<<" first open found = "<<first_open_found);
            LOGDEBUG("POSTDATA = "<<postdata);
            first_open_found=false;
            counter=0;
            if (libjson::is_valid(postdata)) {
              LOGDEBUG("Data is valid");
              inode = libjson::parse(postdata);
              std::string msg = checkJsonEncoding(inode);
              if (msg.length() > 0) {
                JSONNode error(JSON_NODE);
                error.set_name("error");
                error.push_back(JSONNode("code", "attribute_error"));
                error.push_back(JSONNode("description", msg));
                n.push_back(error);
              }else{
                n=save(*db, inode);
              }
            } else {
              JSONNode error(JSON_NODE);

              error.set_name("error");
              error.push_back(JSONNode("code", "parse_error"));
              error.push_back(JSONNode("description", "no valid json format given"));
              n.push_back(error);
              
            }
          } catch (std::exception &ex) {
            LOGDEBUG(ex.what());
            JSONNode error(JSON_NODE);
            error.set_name("error");
            error.push_back(JSONNode("code", "internal_error"));
            error.push_back(JSONNode("description", "internal error ouccured"));
            n.push_back(error);
            
          }

        } else if (req->getMethod() == "GET") {
          std::string id = req->getParameter("id");
          bool delflag = req->hasParameter("delete");
          bool stopflag = req->hasParameter("stop");
          bool fullflag = req->hasParameter("full");
          if (delflag) {
            if (id.length() > 0) {
              LOGDEBUG("loading encoding data for id " << id);
              litesql::DataSource<db::Job>s = litesql::select<db::Job > (*db, db::Job::Uuid == id);
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
                  job.status = "deleted";
                  job.update();
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

          } else if (stopflag) {
            if (id.length() > 0) {
              //LOGDEBUG("loading encoding data for id " << id);
              litesql::DataSource<db::Job>s = litesql::select<db::Job > (*db, db::Job::Uuid == id);
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
          } else if (id.length() > 0) {
            //LOGDEBUG("loading encoding data for id " << id);
            litesql::DataSource<db::Job>s = litesql::select<db::Job > (*db, db::Job::Uuid == id && db::Job::Status != "deleted");
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
          } else {
            LOGDEBUG("listing all encodings")
            std::vector<db::Job> jobs = litesql::select<db::Job > (*db, db::Job::Status != "deleted").orderBy(db::Job::Id, false).all();
            //JSONNode n(JSON_NODE);
            JSONNode c(JSON_ARRAY);
            c.set_name("data");
            std::vector<db::Job>::iterator jobit = jobs.begin();
            for (; jobit != jobs.end(); jobit++) {
              c.push_back(JsonEncoding((*jobit), !fullflag));
            }
            n.push_back(c);
          }
        }else if (req->getMethod() == "PUT") {
            std::string putdata;
            req->getInputstream()->read(putdata);
            LOGDEBUG("PUTDATA = "<<putdata)
        }else{
              JSONNode error(JSON_NODE);
              error.set_name("error");
              error.push_back(JSONNode("code", "unknown_method"));
              error.push_back(JSONNode("description", std::string("unknown method used to create an encoding, method=").append(req->getMethod())));
              n.push_back(error);
          
        }
        n.push_back(JSONNode("requestId", req->getUUID()));
        res->setStatus(ServiceResponse::OK);
        res->getOutputStream()->write(n.write_formatted());
            
      }

      void JSONHandler::handleProfile(ServiceRequest* req, ServiceResponse* res) {
        JSONNode response;
        if (req->getMethod() == "POST") {
          /*check if the incomming data is valid json data*/
          bool valid_data=false;
          JSONNode inode;
          std::string postdata;
          req->getInputstream()->read(postdata);
          //LOGDEBUG("Postdata:"<<postdata);
          try {
            if (libjson::is_valid(postdata)) {
              //LOGDEBUG("Data is valid");
              inode = libjson::parse(postdata);
              std::string msg = checkJsonProfile(inode);
              if (msg.length() > 0) {
                JSONNode error(JSON_NODE);
                error.set_name("error");
                error.push_back(JSONNode("code", "attribute_error"));
                error.push_back(JSONNode("description", msg));
                response.push_back(error);
                //return response;
                
              }else{
                valid_data=true;
              }
            } else {
              JSONNode error(JSON_NODE);
              
              error.set_name("error");
              error.push_back(JSONNode("code", "parse_error"));
              error.push_back(JSONNode("description", "no valid json format given"));
              response.push_back(error);
              //return response;
            }
          } catch (std::exception &ex) {
            LOGDEBUG(ex.what());
            JSONNode error(JSON_NODE);
            error.set_name("error");
            error.push_back(JSONNode("code", "parse_error"));
            error.push_back(JSONNode("description", "no valid json format given"));
            response.push_back(error);
            //return response;
          }

          /*getting the get parameter "id" to find out if a profile will be updated or created*/


          std::string iddata=req->getParameter("id");          
          /*case when "id" data is given, that means a profile update*/
          if(valid_data){
          if (iddata.length() > 0) {
            litesql::DataSource<db::Preset>s = litesql::select<db::Preset > (*db, db::Preset::Uuid == iddata);
            if (s.count() == 1) {
              LOGDEBUG("Update profile");
              db::Preset preset = s.one();
              preset.data = postdata;
              preset.name=inode["name"].as_string();
              preset.update();
              response.push_back(JSONNode("id", iddata));
            } else {
              JSONNode error(JSON_NODE);
              error.set_name("error");
              error.push_back(JSONNode("code", "profile_not_found"));
              error.push_back(JSONNode("description", "profile with the given id not found"));
              response.push_back(error);
            }
          }/*case when no "id" data is given, that means a profile create*/
            else {
              //LOGDEBUG("Create new Profile");
              boost::uuids::uuid uuid = boost::uuids::random_generator()();
              std::string uuidstr = boost::lexical_cast<std::string > (uuid);
              response.push_back(JSONNode("id", uuidstr));
              db::Preset preset(*db);
              preset.data = postdata;
              preset.uuid = uuidstr;
              preset.name = inode["name"].as_string();
              preset.update();
            }
          }

        } else if (req->getMethod() == "GET") {
          std::string id = req->getParameter("id");
          if (id.length() > 0) {
            litesql::DataSource<db::Preset>s = litesql::select<db::Preset > (*db, db::Preset::Uuid == id);
            if (s.count() > 0) {
              db::Preset preset = s.one();
              JSONNode data = libjson::parse(preset.data);
              data.set_name("data");
              if (contains(data, "id")) {
                data["id"] = JSONNode("id", id);
              } else {
                data.push_back(JSONNode("id", id));
              }
              //data.push_back(JSONNode("id", iddata));
              data.preparse();
              //LOGDEBUG(data.write_formatted());
              response = JSONNode(JSON_NODE);
              response.push_back(data);
            } else {
              JSONNode error(JSON_NODE);
              error.set_name("error");
              error.push_back(JSONNode("code", "profile_not_found"));
              error.push_back(JSONNode("description", "profile not found"));
              response.push_back(error);
            }
          } else {
            vector<db::Preset> presets = litesql::select<db::Preset > (*db).all();
            JSONNode c(JSON_ARRAY);
            c.set_name("data");

            foreach(db::Preset preset, presets) {
              JSONNode prnode(JSON_NODE);
              prnode.push_back(JSONNode("id", preset.uuid.value()));
              prnode.push_back(JSONNode("name", preset.name.value()));
              c.push_back(prnode);
            }
            response.push_back(c);
          }

        } else if (req->getMethod() == "DELETE") {
          std::string id = req->getParameter("id");
          if (id.length() > 0) {
            LOGDEBUG("loading preset data for id " << id);
            litesql::DataSource<db::Preset>s = litesql::select<db::Preset > (*db, db::Preset::Uuid == id);
            if (s.count() == 1) {
              db::Preset preset = s.one();
              preset.del();
              JSONNode ok(JSON_NODE);
              ok.set_name("ok");
              ok.push_back(JSONNode("code", "profile_deleted"));
              ok.push_back(JSONNode("description", "profile succesful deleted"));
              response.push_back(ok);
            } else {
              JSONNode error(JSON_NODE);
              error.set_name("error");
              error.push_back(JSONNode("code", "profile_not_found"));
              error.push_back(JSONNode("description", "profile not found"));
              response.push_back(error);
            }
          } else {
            JSONNode error(JSON_NODE);
            error.set_name("error");
            error.push_back(JSONNode("code", "no_id"));
            error.push_back(JSONNode("description", "no id given for delete action"));
            response.push_back(error);

          }
        }
        response.push_back(JSONNode("requestId", req->getUUID()));
        res->setStatus(ServiceResponse::OK);
        res->getOutputStream()->write(response.write_formatted());
      }

      void JSONHandler::handleFormat(ServiceRequest* req, ServiceResponse* res) {
        LOGDEBUG("handle format");
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
        n.push_back(JSONNode("requestId", req->getUUID()));
        res->setStatus(ServiceResponse::OK);
        res->getOutputStream()->write(n.write_formatted());
      }

      void JSONHandler::handleCodec(ServiceRequest* req, ServiceResponse* res) {
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
        n.push_back(JSONNode("requestId", req->getUUID()));
        res->setStatus(ServiceResponse::OK);
        res->getOutputStream()->write(n.write_formatted());
      }

      JSONNode JSONHandler::save(db::HiveDb&db, JSONNode & root) {
        if (contains(root, "outfile")) {
          return save_outfile(db, root);
        } else if (contains(root, "outdir")) {
          return save_outdir(db, root);
        }
        return JSONNode(JSON_NODE);
      }

      JSONNode JSONHandler::save_outdir(db::HiveDb&db, JSONNode & root) {
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

          //org::esb::hive::FileImporter importer;
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


            if (org::esb::io::File(outfile).canWrite()) {
              org::esb::signal::Message msg;
              msg.setProperty("mediaimporter", "import");
              msg.setProperty("file", root["infile"].as_string());
              org::esb::signal::Messenger::getInstance().sendRequest(msg);
              LOGDEBUG("Returned FileId=" << msg.getProperty("fileid"));
              boost::shared_ptr<db::MediaFile> tmf = boost::static_pointer_cast<db::MediaFile > (msg.getVoidProperty("mediafile"));

              db::MediaFile infile = *(tmf.get()); // importer.import(org::esb::io::File(root["infile"].as_string()));
              if (infile.id > 0) {
                org::esb::signal::Message msg_job;
                boost::shared_ptr<db::Preset> pp(new db::Preset(preset));
                msg_job.setProperty("jobcreator", "create").
                        setProperty("mediafile", tmf).
                        setProperty("preset", pp).
                        setProperty("outfile", root["outfile"].as_string());
                org::esb::signal::Messenger::getInstance().sendRequest(msg_job);
                LOGDEBUG("Returned job Object:" << msg_job.getProperty("jobid"));

                int id = atoi(msg_job.getProperty("jobid").c_str()); //org::esb::hive::JobUtil::createJob(infile, preset, root["outfile"].as_string());
                LOGDEBUG("Returned job id:" << id);
                db::Job pre = litesql::select<db::Job > (db, db::Job::Id == id).one();
                ids.push_back(JSONNode("id", pre.uuid.value()));
              } else {
                JSONNode error(JSON_NODE);
                error.set_name("error");
                error.push_back(JSONNode("code", "infile_not_found"));
                error.push_back(JSONNode("description", std::string("could not open input file with id ").append(ifile.getPath()).append(" for use to create encoding task!")));
                errors.push_back(error);
              }
            } else {
              JSONNode error(JSON_NODE);
              error.set_name("error");
              error.push_back(JSONNode("code", "outfile_not_writable"));
              error.push_back(JSONNode("description", std::string("could not write output file ").append(outfile).append(" for use to create encoding task!")));
              errors.push_back(error);
            }
          }
          if (errors.size() > 0)
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

      JSONNode JSONHandler::save_outfile(db::HiveDb&db, JSONNode & root) {
        LOGDEBUG("root_node:" << root.write_formatted());
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
            org::esb::signal::Message msg;
            msg.setProperty("mediaimporter", "import");
            msg.setProperty("file", root["infile"].as_string());
            org::esb::signal::Messenger::getInstance().sendRequest(msg);
            LOGDEBUG("Returned FileId=" << msg.getProperty("fileid"));
            boost::shared_ptr<db::MediaFile> tmf = boost::static_pointer_cast<db::MediaFile > (msg.getVoidProperty("mediafile"));

            //org::esb::hive::FileImporter importer;
            db::MediaFile infile = *(tmf.get()); // importer.import(org::esb::io::File(root["infile"].as_string()));
            if (infile.id > 0) {
              org::esb::signal::Message msg_job;
              boost::shared_ptr<db::Preset> pp(new db::Preset(preset));
              msg_job.setProperty("jobcreator", "create").
                      setProperty("mediafile", tmf).
                      setProperty("preset", pp).
                      setProperty("outfile", root["outfile"].as_string());
              org::esb::signal::Messenger::getInstance().sendRequest(msg_job);
              LOGDEBUG("Returned job Object:" << msg_job.getProperty("jobid"));

              int id = atoi(msg_job.getProperty("jobid").c_str()); //org::esb::hive::JobUtil::createJob(infile, preset, root["outfile"].as_string());
              LOGDEBUG("Returned job id:" << id);
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

    }
  }
}
