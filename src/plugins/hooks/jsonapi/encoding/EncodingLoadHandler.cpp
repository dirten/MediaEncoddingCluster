#include "org/esb/db/hivedb.hpp"
#include "org/esb/core/WebHookPlugin.h"
#include "org/esb/core/PluginContext.h"

#include "org/esb/libjson/libjson.h"
#include "org/esb/libjson/JSONResult.h"

#include "../exports.h"

#include "org/esb/core/StorageEngine.h"
#include "org/esb/db/Job.h"

using org::esb::model::Job;

class JSONAPI_EXPORT EncodingLoadHandler : public org::esb::core::WebHookPlugin {
public:

  void handle(org::esb::core::http::HTTPServerRequest&req, org::esb::core::http::HTTPServerResponse&res) {

    JSONResult result(req);
    std::string id = req.get("encodingid");

    Job job=getContext()->getStorageEngine()->getJobByUUID(id);
    if(job.uuid==id){
      JSONNode entry(JSON_NODE);
      entry.set_name("data");
      //entry.push_back(JSONNode("flowname", job.graphname.value()));
      if (job.graph.length() > 0 && job.graph != "NULL") {
        JSONNode g = libjson::parse(job.graph);
        g.set_name("flow");
        entry.push_back(g);
      } else {
        JSONNode g(JSON_NODE);
        g.set_name("flow");
        entry.push_back(g);
      }
      JSONNode fstatus(JSON_NODE);
      fstatus.set_name("status");

      fstatus.push_back(JSONNode("uuid", job.uuid));
      fstatus.push_back(JSONNode("submitted", job.created));
      fstatus.push_back(JSONNode("begintime", job.begintime));
      fstatus.push_back(JSONNode("endtime", job.endtime));
      fstatus.push_back(JSONNode("progress", job.progress));
      //entry.push_back(JSONNode("fps", job.fps.value()));
      fstatus.push_back(JSONNode("infile", job.infile));
      fstatus.push_back(JSONNode("outfile", job.outfile));
      //fstatus.push_back(JSONNode("statustext", job.getStatusText()));
      fstatus.push_back(JSONNode("statuscode", job.status));
      try {
        if (job.graphstatus.length() > 0) {
          JSONNode gstatus = libjson::parse(job.graphstatus);

          int size=gstatus.size();
          JSONNode g(JSON_ARRAY);
          g.set_name("tasks");
          for(int a=0;a<size;a++){
            JSONNode ientry=gstatus.pop_back(0);
            ientry.push_back(JSONNode("uid",ientry.name()));
            g.push_back(ientry);
          }
          fstatus.push_back(g);
        }else{
          JSONNode g(JSON_ARRAY);
          g.set_name("tasks");
          fstatus.push_back(g);
        }
      } catch (std::exception & ex) {
        LOGERROR("ERROR:" << ex.what());
      }
      entry.push_back(fstatus);

      result.push_back(entry);

    }else {
      //res.setStatusAndReason(res.HTTP_NOT_FOUND, "Encoding not found with id : ");
      result.setStatus(res.HTTP_NOT_FOUND, "encoding not found");
    }
    /*
    db::HiveDb db("sqlite3", req.get("db.url"));


    litesql::DataSource<db::Job>s = litesql::select<db::Job > (db, db::Job::Uuid == id && db::Job::Status != db::Job::Status::Deleted);
    if (s.count() > 0) {
      db::Job job = s.one();
      JSONNode entry(JSON_NODE);
      entry.set_name("data");
      //entry.push_back(JSONNode("flowname", job.graphname.value()));
      if (job.graph.value().length() > 0 && job.graph.value() != "NULL") {
        JSONNode g = libjson::parse(job.graph.value());
        g.set_name("flow");
        entry.push_back(g);
      } else {
        JSONNode g(JSON_NODE);
        g.set_name("flow");
        entry.push_back(g);
      }
      JSONNode fstatus(JSON_NODE);
      fstatus.set_name("status");
      
      fstatus.push_back(JSONNode("uuid", job.uuid.value()));
      fstatus.push_back(JSONNode("submitted", job.created));
      fstatus.push_back(JSONNode("begintime", job.begintime));
      fstatus.push_back(JSONNode("endtime", job.endtime));
      fstatus.push_back(JSONNode("progress", job.progress.value()));
      //entry.push_back(JSONNode("fps", job.fps.value()));
      fstatus.push_back(JSONNode("infile", job.infile.value()));
      fstatus.push_back(JSONNode("outfile", job.outfile.value()));
      fstatus.push_back(JSONNode("statustext", job.getStatusText()));
      fstatus.push_back(JSONNode("statuscode", job.status.value()));
      try {
        if (job.graphstatus.value().length() > 0) {
          JSONNode gstatus = libjson::parse(job.graphstatus.value());
          
          int size=gstatus.size();
          JSONNode g(JSON_ARRAY);
          g.set_name("tasks");
          for(int a=0;a<size;a++){
            JSONNode ientry=gstatus.pop_back(0);
            ientry.push_back(JSONNode("uid",ientry.name()));
            g.push_back(ientry);
          }
          fstatus.push_back(g);
        }else{
          JSONNode g(JSON_ARRAY);
          g.set_name("tasks");
          fstatus.push_back(g);
        }
      } catch (std::exception & ex) {
        LOGERROR("ERROR:" << ex.what());
      }
      entry.push_back(fstatus);

      result.push_back(entry);

    } else {
      //res.setStatusAndReason(res.HTTP_NOT_FOUND, "Encoding not found with id : ");
      result.setStatus(res.HTTP_NOT_FOUND, "encoding not found");
    }*/
    res.setContentType("text/plain");
    std::ostream& ostr = res.send();
    ostr << result.write_formatted();
  }

};
REGISTER_WEB_HOOK("/api/v1/encoding/{encodingid}$", GET, EncodingLoadHandler)








