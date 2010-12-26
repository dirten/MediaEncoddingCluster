/* 
 * File:   ProtocolDispatcher.h
 * Author: HoelscJ
 *
 * Created on 1. Oktober 2010, 14:32
 */

#ifndef PROTOCOLDISPATCHER_H
#define	PROTOCOLDISPATCHER_H
#include <string>
#include "org/esb/lang/Ptr.h"
#include "org/esb/net/TcpSocket.h"
#include "NodeCmdHandler.h"
#include <list>
namespace org {
  namespace esb {
    namespace grid {

      class ProtocolDispatcher {
      public:
        ProtocolDispatcher(Ptr<org::esb::net::TcpSocket>);
        void dispatch(std::string datacmd);
        virtual ~ProtocolDispatcher();
        void run();
      private:
        Ptr<org::esb::net::TcpSocket> _socket;
        std::list<Ptr<NodeCmdHandler> > _handler_list;
      };
    }
  }
}

#endif	/* PROTOCOLDISPATCHER_H */

