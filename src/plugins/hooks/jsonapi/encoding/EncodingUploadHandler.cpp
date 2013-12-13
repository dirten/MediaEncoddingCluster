
#include "org/esb/db/hivedb.hpp"
#include "org/esb/core/WebHookPlugin.h"
#include "org/esb/libjson/libjson.h"
#include "org/esb/io/File.h"
#include "org/esb/av/FormatInputStream.h"
#include "org/esb/util/Foreach.h"
#include "Poco/Net/PartHandler.h"
#include "Poco/Net/HTMLForm.h"
#include "Poco/CountingStream.h"
#include "Poco/NullStream.h"
#include "Poco/StreamCopier.h"
#include "Poco/RegularExpression.h"
#include "Poco/String.h"
#include "../JSONResult.h"
#include "../exports.h"
#include "org/esb/util/UUID.h"
#include "org/esb/config/config.h"
#include "org/esb/util/Log.h"

class EncodingUploadPartHandler : public Poco::Net::PartHandler {
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
    _length=cistr.chars();
  }

  std::string getData() {
    return _data;
  }
  int getLength(){
    return _length;
  }

private:
  std::string _data;
  int _length;
};
class JSONAPI_EXPORT EncodingUploadHandler : public org::esb::core::WebHookPlugin {

   db::HiveDb _db;
  public:
   EncodingUploadHandler() : _db(db::HiveDb("sqlite3", org::esb::config::Config::get("db.url"))){

   }
    void handle(org::esb::core::http::HTTPServerRequest&req, org::esb::core::http::HTTPServerResponse&res) {
      LOGDEBUG("handle upload");
      JSONResult result(req);
      EncodingUploadPartHandler partHandler;
      Poco::Net::HTMLForm form(req, req.stream(), partHandler);

      res.setChunkedTransferEncoding(true);
      res.setContentType("text/plain");
      LOGDEBUG("file size="<<partHandler.getLength())
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
REGISTER_WEB_HOOK("/api/v1/encoding/{profileid}$", POST, EncodingUploadHandler)








