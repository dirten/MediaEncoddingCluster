#ifndef WEBHOOKPROXY_H
#define WEBHOOKPROXY_H
#include "org/esb/core/WebHookPlugin.h"
#include "org/esb/util/Log.h"
namespace org{
  namespace esb{
    namespace core{
      class WebHookProxy : public org::esb::core::WebHookPlugin
      {
          classlogger("org.esb.core.WebHookProxy")
        public:
          WebHookProxy();
          void handle(org::esb::core::http::HTTPServerRequest&req, org::esb::core::http::HTTPServerResponse&res);
      };

    }
  }
}

#endif // WEBHOOKPROXY_H
