#include "org/esb/db/hivedb.hpp"
#include "org/esb/core/WebHookPlugin.h"
#include "org/esb/libjson/libjson.h"
#include "org/esb/io/File.h"
#include "org/esb/av/FormatInputStream.h"
#include "org/esb/util/Foreach.h"
#include "../JSONResult.h"
#include "../exports.h"

class JSONAPI_EXPORT ProfileLoadHandler : public org::esb::core::WebHookPlugin {
public:

  void handle(org::esb::core::http::HTTPServerRequest&req, org::esb::core::http::HTTPServerResponse&res) {


    JSONResult result(req);
    db::HiveDb db("sqlite3", req.get("db.url"));
    std::string id=req.get("profileid");
    litesql::DataSource<db::Preset>s = litesql::select<db::Preset > (db, db::Preset::Uuid == id);
    if (s.count() > 0) {
      try {
        db::Preset preset = s.one();
        JSONNode data = libjson::parse(preset.data);

        data.set_name("data");
        if (data.contains("id")) {
          data.pop_back("id");
        }
        if(data.contains("uuid")){
          data["uuid"] = JSONNode("uuid", id);
        }else{
          data.push_back(JSONNode("uuid", id));
        }



        /*convert old string values into number values*/
        /*
        JSONNode video=data["video"];
        if(video.contains("b")){
          video["b"]=video["b"].as_int();
        }
        data.pop_back("video");
        data.push_back(video);
        */

        JSONNode audio=data["audio"];
        if(audio.contains("ab")){
          audio["ab"]=atoi(audio["ab"].as_string().c_str());
        }
        if(audio.contains("ac")){
          audio["ac"]=atoi(audio["ac"].as_string().c_str());
        }
        if(audio.contains("ar")){
          audio["ar"]=atoi(audio["ar"].as_string().c_str());
        }
        data.pop_back("audio");
        data.push_back(audio);

        result.setData(data);
        result.setStatus("ok");
      } catch (std::exception &ex) {
        res.setStatusAndReason(res.HTTP_NOT_FOUND, "no valid json format from database");
        result.setStatus("error","no valid json format from database");
        LOGDEBUG(ex.what());
      }
    } else {
      res.setStatusAndReason(res.HTTP_NOT_FOUND, "Profile not found with id : "+id);
      result.setStatus("error","profile not found");
    }
    //req.response().setContentType("text/plain");
    res.setContentType("text/plain");
    std::ostream& ostr = res.send();
    ostr << result.write_formatted();
  }

};
REGISTER_WEB_HOOK("/api/v1/profile/{profileid}(?:\\?.*)?$", GET, ProfileLoadHandler)







