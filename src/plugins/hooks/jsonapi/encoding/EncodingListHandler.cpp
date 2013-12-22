#include "org/esb/db/hivedb.hpp"
#include "org/esb/core/WebHookPlugin.h"
#include "org/esb/libjson/libjson.h"
#include "org/esb/libjson/JSONResult.h"
#include "org/esb/io/File.h"
#include "org/esb/av/FormatInputStream.h"
#include "org/esb/util/Foreach.h"
#include "../exports.h"


class JSONAPI_EXPORT EncodingListHandler : public org::esb::core::WebHookPlugin {
public:
  void handle(org::esb::core::http::HTTPServerRequest&req, org::esb::core::http::HTTPServerResponse&res) {
    JSONResult result(req);
    db::HiveDb db("sqlite3", req.get("db.url"));
    JSONNode c(JSON_ARRAY);
    c.set_name("data");
    std::vector<db::Job> jobs = litesql::select<db::Job > (db, db::Job::Status != db::Job::Status::Deleted).orderBy(db::Job::Id, false).all();
    std::vector<db::Job>::iterator jobit = jobs.begin();
    for (; jobit != jobs.end(); jobit++) {
      db::Job job=(*jobit);
      JSONNode entry(JSON_NODE);
      entry.push_back(JSONNode("id",job.uuid.value()));
      entry.push_back(JSONNode("submitted", job.created));
      entry.push_back(JSONNode("progress", job.progress.value()));
      entry.push_back(JSONNode("infile", job.infile.value()));
      entry.push_back(JSONNode("status", job.getStatusText()));
      entry.push_back(JSONNode("statuscode", job.status.value()));
      entry.push_back(JSONNode("flowname", job.graphname.value()));
      c.push_back(entry);
    }
    result.setData(c);
    res.setContentType("text/plain");
    std::ostream& ostr = res.send();
    ostr << result.write_formatted();
  }
};
REGISTER_WEB_HOOK("/api/v1/encoding/?$", GET, EncodingListHandler)
