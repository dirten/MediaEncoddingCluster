#include "org/esb/db/hivedb.hpp"
#include "org/esb/core/WebHookPlugin.h"
#include "org/esb/libjson/libjson.h"
#include "org/esb/io/File.h"
#include "org/esb/av/FormatInputStream.h"
#include "org/esb/util/Foreach.h"
#include "../JSONResult.h"
#include "../exports.h"

class JSONAPI_EXPORT EncodingCreateHandler : public org::esb::core::WebHookPlugin {
public:

  void handle(org::esb::core::http::HTTPServerRequest&req, org::esb::core::http::HTTPServerResponse&res) {


    JSONResult result;
    result.push_back(JSONNode("requestUUID", req.get("requestUUID")));



    res.setContentType("text/plain");
    std::ostream& ostr = res.send();
    ostr << result.write_formatted();
  }

};
//REGISTER_WEB_HOOK("/api/v1/encoding/?$", PUT, EncodingCreateHandler);
//REGISTER_WEB_HOOK("/api/v1/encoding/?$", POST, EncodingCreateHandler);








