/* 
 * File:   NodeCmdHandler.h
 * Author: HoelscJ
 *
 * Created on 1. Oktober 2010, 13:28
 */

#ifndef NODECMDHANDLER_H
#define	NODECMDHANDLER_H

#include "org/esb/lang/Ptr.h"
#include "org/esb/net/TcpSocket.h"

namespace org {
  namespace esb {
    namespace grid {

      class NodeCmdHandler {
      public:
        NodeCmdHandler(Ptr<org::esb::net::TcpSocket> s):_socket(s){};
        virtual bool handleCommand(std::string & cmddata)=0;
        virtual ~NodeCmdHandler(){};
      private:
      protected:
        Ptr<org::esb::net::TcpSocket> _socket;
      };
    }
  }
}

#endif	/* NODECMDHANDLER_H */

