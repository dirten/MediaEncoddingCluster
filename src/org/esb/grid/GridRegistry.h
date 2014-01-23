#ifndef GRIDREGISTRY_H
#define GRIDREGISTRY_H

#include "org/esb/signal/Message.h"
#include "org/esb/signal/MessageListener.h"
#include "org/esb/signal/Messenger.h"
#include "org/esb/util/Log.h"
#include "org/esb/lang/Ptr.h"

#include "GridNode.h"
#include "list"
namespace org{
  namespace esb{
    namespace grid{

      class GridRegistry: public org::esb::signal::MessageListener
      {
          classlogger("org.esb.grid.GridRegistry")
        public:
          static void init();
          static void addNode(GridNode &node);
          static void removeNode(GridNode &node);
          static const std::list<GridNode> nodes();
          static const GridNode node(std::string ip);

    private:
          GridRegistry();

          friend class org::esb::signal::Messenger;
          void onMessage(org::esb::signal::Message &);

          std::list<GridNode> _nodes;
          static GridRegistry * _instance;
      };
    }
  }
}
#endif // GRIDREGISTRY_H
