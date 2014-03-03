#include "org/esb/core/WebHookPlugin.h"
#include "org/esb/libjson/libjson.h"
#include "org/esb/libjson/JSONResult.h"
#include "org/esb/io/File.h"
#include "org/esb/av/FormatInputStream.h"
#include "org/esb/util/Foreach.h"

#include "../exports.h"

class JSONAPI_EXPORT CodecListHandler : public org::esb::core::WebHookPlugin {
public:

  void handle(org::esb::core::http::HTTPServerRequest&req, org::esb::core::http::HTTPServerResponse&res) {

    JSONResult result(req);
    JSONNode c(JSON_ARRAY);
    c.set_name("data");
    AVCodec *p = NULL;
    int a = 0;
    while ((p = av_codec_next(p))) {
      if ( p->encode2) {
        JSONNode cnode(JSON_NODE);
        cnode.push_back(JSONNode("longname", p->long_name));
        cnode.push_back(JSONNode("id", p->name));
        cnode.push_back(JSONNode("type", p->type));
        c.push_back(cnode);
      }
    }
    result.push_back(c);
    res.setContentType("text/plain");
    std::ostream& ostr = res.send();
    ostr << result.write_formatted();
  }
};
REGISTER_WEB_HOOK("/api/v1/codec/?$", GET, CodecListHandler)





