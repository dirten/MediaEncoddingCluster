/* 
 * File:   Service.h
 * Author: HoelscJ
 *
 * Created on 5. Oktober 2011, 16:38
 */

#ifndef SERVICE_H
#define	SERVICE_H
#include "org/esb/core/ServicePlugin.h"
#include "NodeResolver.h"
#include "org/esb/util/Log.h"
class NodeAgent;

class Service : public org::esb::core::ServicePlugin {
  classlogger("org.esb.UdpNodeResolver")
public:
  Service();
  void startService();
  void stopService();
  org::esb::core::OptionsDescription getOptionsDescription();

  virtual ~Service();
private:
  org::esb::plugin::NodeResolver * _resolver;
  NodeAgent * _agent;

};
REGISTER_SERVICE("udpnoderesolver", Service)

#endif	/* SERVICE_H */

