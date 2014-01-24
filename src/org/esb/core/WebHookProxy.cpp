#include "WebHookProxy.h"
#include "org/esb/grid/GridRegistry.h"
#include "org/esb/grid/GridNode.h"
#include "org/esb/util/Foreach.h"
#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/HTTPMessage.h"
#include "Poco/URI.h"
#include "Poco/StreamCopier.h"
#include "Poco/StringTokenizer.h"
#include "Poco/URI.h"

using org::esb::core::PluginRegistry;
using org::esb::grid::GridRegistry;
using org::esb::grid::GridNode;

using Poco::Net::HTTPClientSession;
using Poco::Net::HTTPRequest;
using Poco::Net::HTTPResponse;
using Poco::Net::HTTPMessage;
using Poco::StreamCopier;
using Poco::URI;

namespace org{
  namespace esb{
    namespace core{

      WebHookProxy::WebHookProxy()
      {

      }

      void WebHookProxy::handle(org::esb::core::http::HTTPServerRequest&req, org::esb::core::http::HTTPServerResponse&res) {
        URI uri=URI(req.getURI());

        std::string path=uri.getPath();
        std::string param=uri.getQuery();
        std::string ip_address;
        Poco::StringTokenizer param_tokenz(param,"&");
        LOGDEBUG("query parameter : "<<param);
        path+="?";
        for(Poco::StringTokenizer::Iterator it=param_tokenz.begin();it!=param_tokenz.end();it++){
          Poco::StringTokenizer key_tokenz(*it,"=");

          LOGDEBUG("setting query parameter -> key="<<key_tokenz[0]<<" val="<< (key_tokenz.count()>1)?key_tokenz[1]:"");
          if(key_tokenz[0]=="ip"){
            ip_address=(key_tokenz.count()>1)?key_tokenz[1]:"";
            continue;
          }
          //req.add(key_tokenz[0], key_tokenz.count()>1?key_tokenz[1]:"");
          path+=key_tokenz[0]+"=";
          path+=key_tokenz.count()>1?key_tokenz[1]:"";
          //URI::encode(it->second,"",path);
          path+="&";
        }
        LOGDEBUG("calling url : "<<path);

        LOGDEBUG("ip="<<ip_address)
        GridNode node=GridRegistry::node(ip_address);

        HTTPClientSession session(node.getIpAddress().to_string(), atoi(node.getData("webport").c_str()));
        HTTPRequest request(req.getMethod(), path, HTTPMessage::HTTP_1_1);
        session.sendRequest(request);
        HTTPResponse response;

        // print response
        std::istream &is = session.receiveResponse(response);
        //std::cout << response.getStatus() << " " << response.getReason() << std::endl;
        std::ostream& ostr = res.send();
        //ostr << result.write_formatted();
        StreamCopier::copyStream(is, ostr);
      }
    }
  }
}
