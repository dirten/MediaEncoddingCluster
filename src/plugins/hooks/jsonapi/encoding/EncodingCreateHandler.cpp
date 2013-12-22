#include "org/esb/db/hivedb.hpp"
#include "org/esb/core/WebHookPlugin.h"
#include "org/esb/libjson/libjson.h"
#include "org/esb/libjson/JSONResult.h"
#include "org/esb/io/File.h"
#include "org/esb/av/FormatInputStream.h"
#include "org/esb/util/Foreach.h"
#include "Poco/StreamCopier.h"
#include "../exports.h"
#include "org/esb/util/UUID.h"
#include "org/esb/config/config.h"
#include "org/esb/util/Log.h"

class JSONAPI_EXPORT EncodingCreateHandler : public org::esb::core::WebHookPlugin {

   db::HiveDb _db;
  public:
   EncodingCreateHandler() : _db(db::HiveDb("sqlite3", org::esb::config::Config::get("db.url"))){

   }
    void handle(org::esb::core::http::HTTPServerRequest&req, org::esb::core::http::HTTPServerResponse&res) {

      //LOGDEBUG("enter encoding handler")
      JSONResult result(req);
      std::string data;
      Poco::StreamCopier::copyToString(req.stream(), data);
      LOGDEBUG("Data:"+data)
      if (libjson::is_valid(data)) {
        JSONNode inode = libjson::parse(data);


        JSONNode graph(JSON_NODE);
        JSONNode tasks(JSON_ARRAY);
        tasks.set_name("tasks");
        if(!inode.contains("infile") || !inode.contains("outfile") || !inode.contains("preset")){
          res.setStatus("wrong data in json");
          std::ostream& ostr = res.send();
          ostr << result.write_formatted();
          return;
        }
        tasks.push_back(createInfilenode(inode["infile"].as_string()));
        tasks.push_back(createOutfilenode(inode["outfile"].as_string()));

        //db::HiveDb db("sqlite3", req.get("db.url"));

        db::Preset preset = litesql::select<db::Preset > (_db, db::Preset::Uuid == inode["preset"].as_string()).one();
        tasks.push_back(createProfilenode(inode["preset"].as_string(),preset.data));



        graph.push_back(tasks);
        graph.push_back(JSONNode("uuid",org::esb::util::PUUID()));
        graph.push_back(JSONNode("name","bla fasel"));

        createJob("test job",graph.write_formatted(),inode["infile"].as_string());
        //graph.set_name("graph");
        LOGDEBUG(graph.write_formatted());
      }


      res.setContentType("text/plain");
      std::ostream& ostr = res.send();
      ostr << result.write_formatted();
    }
  private:
    JSONNode createInfilenode(std::string filename){
      JSONNode result;
      result.push_back(JSONNode("name","HTTPPullSource"));
      result.push_back(JSONNode("uid",1));

      JSONNode data(JSON_NODE);
      data.set_name("data");
      data.push_back(JSONNode("srcurl",filename));
      result.push_back(data);

      JSONNode links(JSON_ARRAY);
      links.set_name("linksTo");
      links.push_back(JSONNode("","3"));
      result.push_back(links);


      return result;
    }

    JSONNode createOutfilenode(std::string filename){
      JSONNode result;
      result.push_back(JSONNode("name","OutputTask"));
      result.push_back(JSONNode("uid",2));

      JSONNode data(JSON_NODE);
      data.set_name("data");
      data.push_back(JSONNode("outfile",filename));
      result.push_back(data);


      return result;
    }
    JSONNode createProfilenode(std::string uuid, std::string jdata){

      JSONNode result;
      result.push_back(JSONNode("name","EncodingTask"));
      result.push_back(JSONNode("uid",3));

      JSONNode data=libjson::parse(jdata);
      data.set_name("data");
      result.push_back(data);

      JSONNode links(JSON_ARRAY);
      links.set_name("linksTo");
      links.push_back(JSONNode("","2"));
      result.push_back(links);

      return result;
    }

    std::string createJob(std::string graphname, std::string graph, std::string infile) {
      db::Job job(_db);
      job.uuid = (std::string)org::esb::util::PUUID();
      job.status = db::Job::Status::Waiting;
      job.graph = graph;
      job.graphname = graphname;
      job.infile = infile;
      job.created = 0;
      job.update();
      return job.uuid;
    }

};
//REGISTER_WEB_HOOK("/api/v1/encoding/?$", PUT, EncodingCreateHandler)
REGISTER_WEB_HOOK("/api/v1/encoding/?$", POST, EncodingCreateHandler)








