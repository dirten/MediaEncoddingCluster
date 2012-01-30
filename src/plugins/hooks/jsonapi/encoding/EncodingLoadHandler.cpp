#include "org/esb/db/hivedb.hpp"
#include "org/esb/core/WebHookPlugin.h"
#include "org/esb/libjson/libjson.h"
#include "org/esb/io/File.h"
#include "org/esb/av/FormatInputStream.h"
#include "org/esb/util/Foreach.h"
#include "../JSONResult.h"
#include "../exports.h"

class JSONAPI_EXPORT EncodingLoadHandler : public org::esb::core::WebHookPlugin {
public:

  void handle(org::esb::core::http::HTTPServerRequest&req, org::esb::core::http::HTTPServerResponse&res) {


    JSONResult result(req.get("requestUUID"));
    db::HiveDb db("sqlite3", req.get("db.url"));
    std::string id = req.get("encodingid");
    litesql::DataSource<db::Job>s = litesql::select<db::Job > (db, db::Job::Uuid == id && db::Job::Status != db::Job::Status::Deleted);
    if (s.count() > 0) {
      //LOGDEBUG("Encoding found");
      db::Job job = s.one();
      JSONNode entry(JSON_NODE);
      entry.set_name("data");
      entry.push_back(JSONNode("id",job.uuid.value()));
      entry.push_back(JSONNode("created", job.created));
      entry.push_back(JSONNode("begintime", job.begintime));
      entry.push_back(JSONNode("endtime", job.endtime));
      entry.push_back(JSONNode("progress", job.progress.value()));
      entry.push_back(JSONNode("fps", job.fps.value()));
      entry.push_back(JSONNode("infile", job.infile.value()));
      entry.push_back(JSONNode("outfile", job.outfile.value()));
      entry.push_back(JSONNode("status", job.getStatusText()));
      entry.push_back(JSONNode("statuscode", job.status.value()));
      entry.push_back(JSONNode("graphname", job.graphname.value()));
      entry.push_back(JSONNode("infile", job.infile.value()));
      result.push_back(entry);
    } else {
      res.setStatusAndReason(res.HTTP_NOT_FOUND, "Encoding not found with id : ");
      result.setStatus("error","encoding not found");
    }
    //req.response().setContentType("text/plain");
    res.setContentType("text/plain");
    std::ostream& ostr = res.send();
    ostr << result.write_formatted();
  }

};
REGISTER_WEB_HOOK("/api/v1/encoding/{encodingid}$", GET, EncodingLoadHandler);








