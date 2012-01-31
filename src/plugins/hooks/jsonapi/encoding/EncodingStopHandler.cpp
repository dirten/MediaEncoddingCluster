#include "org/esb/db/hivedb.hpp"
#include "org/esb/core/WebHookPlugin.h"
#include "org/esb/libjson/libjson.h"
#include "org/esb/io/File.h"
#include "org/esb/av/FormatInputStream.h"
#include "org/esb/util/Foreach.h"
#include "org/esb/util/StringUtil.h"
#include "org/esb/signal/Messenger.h"
#include "org/esb/signal/Message.h"
#include "../JSONResult.h"
#include "../exports.h"

class JSONAPI_EXPORT EncodingStopHandler : public org::esb::core::WebHookPlugin {
public:

  void handle(org::esb::core::http::HTTPServerRequest&req, org::esb::core::http::HTTPServerResponse&res) {


    JSONResult result(req);
    db::HiveDb db("sqlite3", req.get("db.url"));
    JSONNode c(JSON_ARRAY);
    c.set_name("data");

    std::string id=req.get("encodingid");
    litesql::DataSource<db::Job>s = litesql::select<db::Job > (db, db::Job::Uuid == id);
    if (s.count() == 1) {
      db::Job job = s.one();
      job.status = job.status == db::Job::Status::Processing ? db::Job::Status::Stopping : db::Job::Status::Stopped;
      job.update();
      std::string job_id = org::esb::util::StringUtil::toString(job.id);
      org::esb::signal::Messenger::getInstance().sendMessage(org::esb::signal::Message().setProperty("jobexecutor", "STOP_JOB").setProperty("job_id", job_id));

      result.setStatus("ok","stop encoding succesful signaled");
    } else {
      result.setStatus("error","encoding not found");
      res.setStatusAndReason(res.HTTP_NOT_FOUND, "encoding not found");
    }

    res.setContentType("text/plain");
    std::ostream& ostr = res.send();
    ostr << result.write_formatted();
  }

};
REGISTER_WEB_HOOK("/api/v1/encoding/{encodingid}/stop", POST, EncodingStopHandler);








