/* 
 * File:   ServerHandler.h
 * Author: HoelscJ
 *
 * Created on 22. Dezember 2010, 13:01
 */

#ifndef SERVERHANDLER_H
#define	SERVERHANDLER_H
#include "NodeCmdHandler.h"
namespace org {
  namespace esb {
    namespace grid {

      class ServerHandler: public NodeCmdHandler {
      public:
        ServerHandler(Ptr<org::esb::net::TcpSocket>);
        virtual ~ServerHandler();
        bool handleCommand(std::string & cmddata);
      private:

      };
    }
  }
}

#endif	/* SERVERHANDLER_H */

