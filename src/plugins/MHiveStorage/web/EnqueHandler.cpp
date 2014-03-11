#ifndef ENQUEHOOK_H
#define ENQUEHOOK_H
#include "org/esb/core/WebHookPlugin.h"
#include "org/esb/core/PluginContext.h"
#include "org/esb/util/Serializing.h"
#include "org/esb/hive/Environment.h"
#include "org/esb/hive/job/ProcessUnit.h"
#include "Poco/Net/PartHandler.h"
#include "../services/engines/Simple.h"
using org::esb::util::Serializing;
using org::esb::hive::Environment;
using mhivestorage::engines::Simple;
using org::esb::core::PluginContext;
namespace mhivestorage{
  namespace web{

    class EncodingUploadPartHandler : public Poco::Net::PartHandler {
      public:
        EncodingUploadPartHandler(PluginContext * ctx){
          _ctx=ctx;
        }

        void handlePart(const Poco::Net::MessageHeader&header, std::istream&stream) {
          if (header.has("Content-Disposition")) {
            std::string disp;
            Poco::Net::NameValueCollection params;
            Poco::Net::MessageHeader::splitParameters(header["Content-Disposition"], disp, params);
          }
          boost::shared_ptr<org::esb::hive::job::ProcessUnit> unit;
          Serializing::deserialize<boost::archive::text_iarchive>(unit, stream);
          Simple store(_ctx->database, Environment::get("hive.data_path"));
          store.enque(unit);
        }
      private:
        PluginContext * _ctx;
    };

    class EnqueHandler :public org::esb::core::WebHookPlugin
    {
      public:
        EnqueHandler(){

        }

        void handle(org::esb::core::http::HTTPServerRequest&req, org::esb::core::http::HTTPServerResponse&res){
          getContext()->database;
        }
    };
  }
}
#endif // ENQUEHOOK_H
