#include "org/esb/db/hivedb.hpp"
#include "org/esb/core/WebHookPlugin.h"
#include "org/esb/libjson/libjson.h"
#include "org/esb/io/File.h"
#include "org/esb/av/FormatInputStream.h"
#include "org/esb/util/Foreach.h"
#include "../JSONResult.h"
#include "../exports.h"

class JSONAPI_EXPORT EncodingStatusHandler : public org::esb::core::WebHookPlugin {
public:

  void handle(org::esb::core::http::HTTPServerRequest&req, org::esb::core::http::HTTPServerResponse&res) {


    JSONResult result(req);
    db::HiveDb db("sqlite3", req.get("db.url"));
    std::string id = req.get("encodingid");
    litesql::DataSource<db::Job>s = litesql::select<db::Job > (db, db::Job::Uuid == id && db::Job::Status != db::Job::Status::Deleted);
    if (s.count() > 0) {
      db::Job job = s.one();
      JSONNode data(JSON_ARRAY);
      data.set_name("flowstatus");
      try {
        if (job.graphstatus.value().length() > 0) {
          JSONNode gstatus = libjson::parse(job.graphstatus.value());
          int size=gstatus.size();
          for(int a=0;a<size;a++){
            JSONNode entry=gstatus.pop_back(0);
            entry.push_back(JSONNode("uid",entry.name()));
            data.push_back(entry);
          }
        }
      } catch (std::exception & ex) {
        LOGERROR("ERROR:" << ex.what());
      }
      result.push_back(data);
    } else {
      //res.setStatusAndReason(res.HTTP_NOT_FOUND, "Encoding not found with id : ");
      result.setStatus(res.HTTP_NOT_FOUND, "encoding not found");
    }
    res.setContentType("text/plain");
    std::ostream& ostr = res.send();
    ostr << result.write_formatted();
  }

};
REGISTER_WEB_HOOK("/api/v1/encoding/{encodingid}/status$", GET, EncodingStatusHandler);

