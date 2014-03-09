#ifndef LOCKHOOK_H
#define LOCKHOOK_H
#include "org/esb/core/WebHookPlugin.h"

namespace mhivestorage{
  namespace web{

    class LockHook: public org::esb::core::WebHookPlugin
    {
    public:
      LockHook();
      void handle(org::esb::core::http::HTTPServerRequest&req, org::esb::core::http::HTTPServerResponse&res);
    };
    }
  }
#endif // LOCKHOOK_H
