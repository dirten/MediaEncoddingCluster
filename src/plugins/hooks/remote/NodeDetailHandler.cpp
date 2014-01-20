#include "org/esb/core/WebHookPlugin.h"
#include "org/esb/libjson/JSONResult.h"
#include "org/esb/util/Log.h"

#include "RemoteController.h"
#include "org/esb/grid/GridNode.h"
#include "org/esb/grid/GridRegistry.h"
#include "org/esb/util/Foreach.h"

using org::esb::grid::GridNode;
using org::esb::grid::GridRegistry;
namespace plugin{
  class NodeListHandler : public org::esb::core::WebHookPlugin {

      void handle(org::esb::core::http::HTTPServerRequest&req, org::esb::core::http::HTTPServerResponse&res) {
        JSONResult result(req);
        std::list<GridNode> nodes=GridRegistry::nodes();
        JSONNode c(JSON_ARRAY);
        c.set_name("data");
        foreach(GridNode & node , nodes){
          JSONNode entry(JSON_NODE);
          //entry.set_name("node");
          entry.push_back(JSONNode("ip",node.getIpAddress().to_string()));
          entry.push_back(JSONNode("ip",node.getIpAddress().to_string()));
          entry.push_back(JSONNode("hiveport",node.getData("port")));
          entry.push_back(JSONNode("mgmtport",node.getData("webport")));
          entry.push_back(JSONNode("type",node.getData("type")));

          c.push_back(entry);
        }
        result.setData(c);

        std::ostream& ostr = res.send();
        ostr << result.write_formatted();
      }

  };

  REGISTER_WEB_HOOK("/api/v1/node$", GET, NodeDeatilHandler)

}


