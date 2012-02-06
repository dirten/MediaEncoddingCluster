#include "org/esb/db/hivedb.hpp"
#include "org/esb/core/WebHookPlugin.h"
#include "org/esb/core/GraphParser.h"
#include "org/esb/libjson/libjson.h"
#include "org/esb/config/config.h"
#include "org/esb/util/UUID.h"
#include "org/esb/util/Foreach.h"
#include "Poco/Net/PartHandler.h"
#include "Poco/Net/HTMLForm.h"
#include "Poco/CountingStream.h"
#include "Poco/NullStream.h"
#include "Poco/StreamCopier.h"
#include "Poco/RegularExpression.h"
#include "Poco/String.h"

#include "org/esb/io/File.h"
#include "org/esb/io/FileOutputStream.h"
#include "org/esb/io/FileInputStream.h"
#include "../JSONResult.h"

#include "../exports.h"

class GraphSubmitPartHandler : public Poco::Net::PartHandler {
public:

  void handlePart(const Poco::Net::MessageHeader&header, std::istream&stream) {
    LOGDEBUG("HandlePart");
    if (header.has("Content-Disposition")) {
      LOGDEBUG("Content-Disposition");
      std::string disp;
      Poco::Net::NameValueCollection params;
      Poco::Net::MessageHeader::splitParameters(header["Content-Disposition"], disp, params);
      LOGDEBUG("Param-Name" << params.get("name", "(unnamed)"));
      LOGDEBUG("Param-FileName" << params.get("filename", "(unnamed)"));
    }
    Poco::CountingInputStream cistr(stream);
    Poco::NullOutputStream noss;
    Poco::StreamCopier::copyStream(cistr, noss);
  }

  std::string getData() {
    return _data;
  }
private:
  std::string _data;
};

class JSONAPI_EXPORT GraphSubmitHandler : public org::esb::core::WebHookPlugin {
  db::HiveDb _db;
public:

  GraphSubmitHandler() : _db(db::HiveDb("sqlite3", org::esb::config::Config::get("db.url"))) {

  }

  void handle(org::esb::core::http::HTTPServerRequest&req, org::esb::core::http::HTTPServerResponse&res) {
    JSONResult result(req);
    std::string uuid = req.get("uuid");
    LOGDEBUG("Submit Handler1212:" << uuid);
    //LOGDEBUG("Submit Handler1212:" <<req.get("file"));
    org::esb::io::File f(req.get("hive.graph_path") + "/" + uuid + ".graph");
    if (f.exists()) {
      LOGDEBUG("ContentLength" << req.getContentLength());
      /*when  POST Data arrived*/
      if (req.getContentLength() > 0) {
        GraphSubmitPartHandler partHandler;
        Poco::Net::HTMLForm form(req, req.stream(), partHandler);
      }
      org::esb::io::FileInputStream fis(&f);
      std::string ndata;
      fis.read(ndata);
      LOGDEBUG(ndata);

      /**parsing json file*/
      try {
        std::string filename;
        if(req.has("inputname")){
          filename=req.get("inputname");
        }
        org::esb::core::GraphParser graphparser(ndata, filename);
        //if(inputfile.length()>0)
        //  graphparser.setInfile(inputfile);
        org::esb::io::File infile(graphparser.getInfile());
        JSONNode data(JSON_ARRAY);
        data.set_name("data");
        if (infile.exists() && infile.isDirectory()) {
          org::esb::io::FileList list = infile.listFiles();
          foreach(Ptr<org::esb::io::File> file, list) {
            graphparser.setInfile(file->getPath());
            std::string task_uuid=createJob(graphparser.getName(), graphparser.getGraphString(), file->getPath());
            data.push_back(JSONNode("",task_uuid));
          }
        } else if (infile.exists() && infile.isFile()) {
          std::string task_uuid=createJob(graphparser.getName(), graphparser.getGraphString(), infile.getPath());
          data.push_back(JSONNode("",task_uuid));
        } else {
          std::string reg = infile.getPath();
          reg = Poco::replace(reg, ".", "\\.");
          reg = Poco::replace(reg, "*", ".*");
          Poco::RegularExpression re(reg);
          org::esb::io::File wld(infile.getFilePath());

          org::esb::io::FileList list = wld.listFiles();

          foreach(Ptr<org::esb::io::File> file, list) {
            if (re.match(file->getPath())) {
              LOGDEBUG("File Found:" << file->getPath());
              graphparser.setInfile(file->getPath());
              std::string task_uuid=createJob(graphparser.getName(), graphparser.getGraphString(), file->getPath());
              data.push_back(JSONNode("",task_uuid));
            }
          }
        }
        result.push_back(data);
        //org::esb::core::Graph::createJob(list, getContext()->database);
        //result.push_back(JSONNode("status", "ok"));
      } catch (org::esb::core::GraphException & ex) {
        result.setStatus(res.HTTP_BAD_REQUEST,ex.what());
        //result.push_back(JSONNode("status", "error"));
        //result.push_back(JSONNode("message", ex.what()));
        //result.push_back(JSONNode("element", ex.getElementId()));
      }



      //result.push_back(JSONNode("status", "ok"));
    } else {
        result.setStatus(res.HTTP_NOT_FOUND, "graph for submission not found!");
      //result.push_back(JSONNode("status", "error"));
      //result.push_back(JSONNode("message", "graph for submission not found!"));
      //res.setStatusAndReason(res.HTTP_NOT_FOUND, "graph for submission not found!");
    }
    std::ostream& ostr = res.send();
    ostr << result.write_formatted();
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
REGISTER_WEB_HOOK("/api/v1/flow/{uuid}/{inputname}", POST, GraphSubmitHandler);



