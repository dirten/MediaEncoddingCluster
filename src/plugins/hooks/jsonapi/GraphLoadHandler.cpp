#include "org/esb/core/WebHookPlugin.h"
#include "exports.h"
#include "org/esb/util/Log.h"

class JSONAPI_EXPORT GraphLoadHandler : public org::esb::core::WebHookPlugin {
public:

  void handle(org::esb::core::http::HTTPServerRequest&req, org::esb::core::http::HTTPServerResponse&res) {
    LOGDEBUG("Loading graph");
    res.setChunkedTransferEncoding(true);
    res.setContentType("text/plain");
    std::ostream& ostr = res.send();
    ostr << req.get("uuid");
  }
};
REGISTER_WEB_HOOK("/api/v1/graph/{uuid}", GET, GraphLoadHandler);