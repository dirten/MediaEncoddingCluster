/* 
 * File:   Service.h
 * Author: HoelscJ
 *
 * Created on 7. Oktober 2011, 15:52
 */

#ifndef LOGSERVICE_H
#define	LOGSERVICE_H
#include "org/esb/core/ServicePlugin.h"
#include "org/esb/net/TcpServerSocket.h"


#include "exports.h"
namespace logserver {

  class Service : public org::esb::core::ServicePlugin {
  public:
    Service();
    virtual ~Service();
    void LOGSERVICE_EXPORT startService();
    void LOGSERVICE_EXPORT stopService();
    org::esb::core::OptionsDescription LOGSERVICE_EXPORT getOptionsDescription();

  private:
    org::esb::net::TcpServerSocket * server;
    void handleClient(org::esb::net::TcpSocket *);
    void run();
  };
  REGISTER_SERVICE("logserver", Service)
}

#endif	/* SERVICE_H */

