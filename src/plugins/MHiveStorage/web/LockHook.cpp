#include "LockHook.h"
namespace mhivestorage{
  namespace web{
    LockHook::LockHook()
    {
    }
    void LockHook::handle(org::esb::core::http::HTTPServerRequest&req, org::esb::core::http::HTTPServerResponse&res){

    }

    REGISTER_WEB_HOOK("/api/v1/storage/lock/{puid}$", POST, LockHook)
    }
  }
