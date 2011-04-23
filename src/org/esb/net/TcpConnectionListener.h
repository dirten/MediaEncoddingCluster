/* 
 * File:   ConnectionListener.h
 * Author: HoelscJ
 *
 * Created on 1. Oktober 2010, 13:11
 */

#ifndef CONNECTIONLISTENER_H
#define	CONNECTIONLISTENER_H
#include "org/esb/lang/Ptr.h"
#include "org/esb/net/TcpSocket.h"
#include "exports.h"
namespace org {
  namespace esb {
    namespace net {

      class NET_EXPORT TcpConnectionListener {
      public:
        virtual void onConnection(Ptr<TcpSocket> socket) = 0;
      };
    }
  }
}

#endif	/* CONNECTIONLISTENER_H */

