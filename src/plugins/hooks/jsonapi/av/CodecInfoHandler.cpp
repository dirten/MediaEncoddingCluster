
#include "org/esb/core/WebHookPlugin.h"
#include "org/esb/libjson/libjson.h"
#include "org/esb/av/FormatInputStream.h"
#include "org/esb/util/Foreach.h"

#include "../JSONResult.h"
#include "../exports.h"

class JSONAPI_EXPORT CodecInfoHandler : public org::esb::core::WebHookPlugin {
public:

  void handle(org::esb::core::http::HTTPServerRequest&req, org::esb::core::http::HTTPServerResponse&res) {
    std::string codecid=req.get("codecid");
    JSONResult result(req);
    AVCodec *p = NULL;
    int a = 0;
    while ((p = av_codec_next(p))) {
      if (p->encode || p->encode2) {
        if(codecid==std::string(p->name))
          break;
      }
    }
    if(p){
    JSONNode cnode(JSON_NODE);
    cnode.set_name("data");
    cnode.push_back(JSONNode("longname", p->long_name));
    cnode.push_back(JSONNode("id", p->name));
    cnode.push_back(JSONNode("type", p->type));

    JSONNode pnode(JSON_ARRAY);
    pnode.set_name("options");

      const AVOption * option = NULL;
      AVClass *clazz =const_cast<AVClass*> (p->priv_class);
      option=clazz->option;
      pnode.push_back(JSONNode("",option->name));
      while (option = av_opt_next(clazz, option)) {
        if (option->offset > 0) {
          int len = 1000;
          char data[1000];
          pnode.push_back(JSONNode("",option->name));
          //av_get_string(p, option->name, NULL, data, len);
        }
      }


     cnode.push_back(pnode);
    result.push_back(cnode);
    }

    res.setContentType("text/plain");
    std::ostream& ostr = res.send();
    ostr << result.write_formatted();
  }
};
REGISTER_WEB_HOOK("/api/v1/codec/{codecid}$", GET, CodecInfoHandler)





